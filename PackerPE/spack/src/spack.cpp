//
#include "spack.h"
#include "sections_packer.h"

#include "tiny_logger.h"
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

std::vector<uint8_t> readFile(const char* filename)
{
  std::streampos fileSize;
  std::ifstream file(filename, std::ios::binary);

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> fileData(static_cast<unsigned int>(fileSize));
  file.read((char*)&fileData[0], fileSize);
  return fileData;
}

void writeFile(const char* filename, uint64_t offset, const char* pData, uint32_t cbData )
{
  std::fstream file(filename, std::fstream::out | std::fstream::binary | std::fstream::in | std::fstream::app);
  file.seekp(offset, std::ios::beg);
  file.write(pData, cbData);
  file.close();
}

uint64_t rebuildMZHeader(PeFilePtr& peFile, std::string& outFileName, std::vector<uint8_t>& sourceFileBuff)
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
    writeFile(outFileName.c_str(), cbMZHead, reinterpret_cast<char*>(&sourceFileBuff.at(cbMZHead)), cbJunkData);
  }

  return cbMZHead + cbJunkData;
}

template<int bits>
void rebuildPEHeader(PeLib::PeFile& peFile, std::string& outFileName, uint64_t& offset)
{
  assert(offset > UINT32_MAX);

  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

  std::vector<PeLib::byte> peHeaderBuff;
  peh.rebuild(peHeaderBuff);

  PeLib::PeHeaderT<bits> pehOut;
  auto cbAfterHeader = peh.size();
  pehOut.read(peHeaderBuff.data(), peHeaderBuff.size(), static_cast<uint32_t>(offset));
  pehOut.write(outFileName, static_cast<uint32_t>(offset));

  offset += pehOut.size();

  // append with aligned zero-bytes
  auto headersSize = peh.getSizeOfHeaders() - offset;
  if (headersSize)
  {
    std::vector<uint8_t> zeroBuff(static_cast<uint32_t>(headersSize));
    writeFile(
      outFileName.c_str()
      , static_cast<uint32_t>(offset)
      , reinterpret_cast<char*>(zeroBuff.data())
      , static_cast<uint32_t>(headersSize));
  }
  offset += headersSize;
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::string outFileName, uint64_t& offset):
    outFileName_(outFileName)
    , offset_(offset)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file, outFileName_, offset_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file, outFileName_, offset_); }
private:
  std::string outFileName_;
  uint64_t& offset_;
};

//------------------------------------------------------------------------

void PackExecutable(std::string srcFileName, std::string outFileName)
{
  auto sourceFileBuff = readFile(srcFileName.c_str());
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

  SectionsPacker sectionsPacker(pef);
  sectionsPacker.ProcessExecutable(sourceFileBuff, outFileBuff);
  //DumpSectionsVisitor peSectionsVisitor(outFileName, sourceFileBuff, offset);
  //pef->visit(peSectionsVisitor);
}