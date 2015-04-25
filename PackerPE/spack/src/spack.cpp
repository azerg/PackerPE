//
#include "spack.h"

#include <cstdint>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "sections_packer.h"
#include "file_utils.h"
#include "tiny_logger.h"

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

uint32_t rebuildMZHeader(PeFilePtr& peFile, std::string& outFileName, std::vector<uint8_t>& sourceFileBuff)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  peFile->mzHeader().rebuild(mzHeadBuffer);

  PeLib::MzHeader mzOut;
  mzOut.read(mzHeadBuffer.data(), mzHeadBuffer.size(), 0);
  mzOut.write(outFileName, 0);

  // append mzHeader with undocumented data located between MZHeader & PEHeader
  auto peOffset = mzOut.getAddressOfPeHeader();
  auto cbMZHead = mzOut.size();
  auto cbJunkData = std::max(peOffset - cbMZHead, static_cast<uint32_t>(0));
  if (cbJunkData)
  {
    file_utils::writeFile(outFileName.c_str(), cbMZHead, reinterpret_cast<char*>(&sourceFileBuff.at(cbMZHead)), cbJunkData);
  }

  return cbMZHead + cbJunkData;
}

template<int bits>
void rebuildPEHeader(PeLib::PeFile& peFile, std::string& outFileName, uint32_t& offset)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

  std::vector<PeLib::byte> peHeaderBuff;
  peh.rebuild(peHeaderBuff);

  PeLib::PeHeaderT<bits> pehOut;
  auto cbAfterHeader = peh.size();
  pehOut.read(peHeaderBuff.data(), peHeaderBuff.size(), offset);
  pehOut.write(outFileName, offset);

  offset += pehOut.size();

  // append with aligned zero-bytes
  auto headersSize = peh.getSizeOfHeaders() - offset;
  if (headersSize)
  {
    std::vector<uint8_t> zeroBuff(headersSize);
    file_utils::writeFile(
      outFileName.c_str()
      , offset
      , reinterpret_cast<char*>(zeroBuff.data())
      , headersSize);
  }
  offset += headersSize;
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::string outFileName, uint32_t& offset):
    outFileName_(outFileName)
    , offset_(offset)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file, outFileName_, offset_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file, outFileName_, offset_); }
private:
  std::string outFileName_;
  uint32_t& offset_;
};

//------------------------------------------------------------------------

void PackExecutable(std::string& srcFileName, std::string& outFileName)
{
  try
  {
    auto sourceFileBuff = file_utils::readFile(srcFileName.c_str());
    decltype(sourceFileBuff) outFileBuff;

    if (boost::filesystem::exists(outFileName))
    {
      boost::filesystem::remove(outFileName);
    }

    auto pef = PeLib::openPeFile(srcFileName);
    if (!pef)
    {
      std::cout << "Invalid PE File" << std::endl;
      return;
    }

    pef->readMzHeader();
    auto offset = rebuildMZHeader(pef, outFileName, sourceFileBuff);

    pef->readPeHeader();
    DumpPeHeaderVisitor peVisitor(outFileName, offset);
    pef->visit(peVisitor);

    SectionsPacker sectionsPacker(srcFileName);
    sectionsPacker.ProcessExecutable(outFileName);
  }
  catch (std::runtime_error& err)
  {
    std::cout << err.what();
  }
}