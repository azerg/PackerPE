//
#include "spack.h"
#include "PeLib.h"

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
    writeFile(outFileName.c_str(), offset, reinterpret_cast<char*>(zeroBuff.data()), headersSize);
  }
  offset += headersSize;
}

template<int bits>
void rebuildSections(PeLib::PeFile& peFile, std::string& outFileName, std::vector<uint8_t>& sourceFileBuff, uint64_t& offset)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();
  uint32_t cbSections{};

  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    peh.getSectionName(idxSection);

    auto cbSection = peh.getSizeOfRawData(idxSection);

    boost::filesystem::path path(outFileName);
    path = path.parent_path();
    path /= peh.getSectionName(idxSection);

    writeFile(
      outFileName.c_str()
      , offset
      , reinterpret_cast<char*>(&sourceFileBuff.at(peh.getPointerToRawData(idxSection)))
      , cbSection);

    cbSections += cbSection;
  }

  peh.writeSections(outFileName);

  offset += cbSections;
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

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(std::string outFileName, std::vector<uint8_t>& sourceFileBuff, uint64_t& offset) :
    outFileName_(outFileName)
    , offset_(offset)
    , sourceFileBuff_(sourceFileBuff)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildSections<32>(file, outFileName_, sourceFileBuff_, offset_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildSections<64>(file, outFileName_, sourceFileBuff_, offset_); }
private:
  std::vector<uint8_t> sourceFileBuff_;
  std::string outFileName_;
  uint64_t& offset_;
};

//------------------------------------------------------------------------

void PackExecutable(std::string srcFileName, std::string outFileName)
{
  auto sourceFileBuff = readFile(srcFileName.c_str());

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

  DumpSectionsVisitor peSectionsVisitor(outFileName, sourceFileBuff, offset);
  pef->visit(peSectionsVisitor);
}