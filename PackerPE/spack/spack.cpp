//
#include "spack.h"
#include "PeLib.h"

#include "tiny_logger.h"
#include <cstdint>
#include <algorithm>
#include <fstream>

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
  ..
  std::streampos fileSize;
  std::fstream file(filename, std::ios_base::out | std::ios::binary);
  file.seekp(offset, std::ios::beg);
  file.write(pData, cbData);
  //file.seekp(0, std::ios::beg);
  file.close();
}

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
    writeFile(outFileName.c_str(), cbMZHead, reinterpret_cast<char*>(&sourceFileBuff.at(cbMZHead)), cbJunkData);
  }

  return cbMZHead + cbJunkData;
}

template<int bits>
uint32_t rebuildPEHeader(PeLib::PeFile& peFile, std::string& outFileName, uint32_t offset)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

  std::vector<PeLib::byte> peHeaderBuff;
  peh.rebuild(peHeaderBuff);

  PeLib::PeHeaderT<bits> pehOut;
  pehOut.read(peHeaderBuff.data(), peHeaderBuff.size(), offset);
  pehOut.write(outFileName, offset);
  return offset + pehOut.size();
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::string outFileName, uint64_t offset):
    outFileName_(outFileName)
    , offset_(offset)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file, outFileName_, offset_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file, outFileName_, offset_); }
private:
  std::string outFileName_;
  uint64_t offset_;
};

//------------------------------------------------------------------------

void PackExecutable(std::string srcFileName, std::string outFileName)
{
  auto sourceFileBuff = readFile(srcFileName.c_str());

  auto pef = PeLib::openPeFile(srcFileName);
  if (!pef)
  {
    std::cout << "Invalid PE File" << std::endl;
    return;
  }

  pef->readMzHeader();
  auto offset = rebuildMZHeader(pef, outFileName, sourceFileBuff);

  dump(centerOutput("----------------------------------------------"));
  dump("");

  pef->readPeHeader();
  DumpPeHeaderVisitor peVisitor(outFileName, offset);
  pef->visit(peVisitor);
}