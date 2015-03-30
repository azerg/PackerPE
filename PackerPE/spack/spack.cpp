//
#include "spack.h"
#include "PeLib.h"

#include <cstdint>

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

uint64_t rebuildMZHeader(PeFilePtr& peFile, std::string& outFileName)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  peFile->mzHeader().rebuild(mzHeadBuffer);

  PeLib::MzHeader mzOut;
  mzOut.read(mzHeadBuffer.data(), mzHeadBuffer.size(), 0);
  mzOut.write(outFileName, 0);

  return mzOut.size();
}

template<int bits>
uint64_t rebuildPEHeader(PeLib::PeFile& peFile, std::string& outFileName, uint64_t offset)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

  std::vector<PeLib::byte> peHeaderBuff;
  peh.rebuild(peHeaderBuff);

  PeLib::PeHeaderT<bits> pehOut;
  pehOut.read(peHeaderBuff.data(), peHeaderBuff.size(), offset);

  return offset + pehOut.size();
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::string outFileName, uint64_t offset):
    outFileName_(outFileName)
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
  auto pef = PeLib::openPeFile(srcFileName);
  if (!pef)
  {
    std::cout << "Invalid PE File" << std::endl;
    return;
  }

  pef->readMzHeader();
  auto offset = rebuildMZHeader(pef, outFileName);

  pef->readPeHeader();
  DumpPeHeaderVisitor peVisitor(outFileName, offset);
  pef->visit(peVisitor);
}