//
#include "spack.h"
#include "PeLib.h"

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

void rebuildMZHeader(PeFilePtr& peFile, std::string& outFileName)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  peFile->mzHeader().rebuild(mzHeadBuffer);

  PeLib::MzHeader mzOut;
  mzOut.read(mzHeadBuffer.data(), mzHeadBuffer.size(), 0);
  mzOut.write(outFileName, 0);
}

template<int bits>
void rebuildPEHeader(PeLib::PeFile& peFile, std::string& outFileName)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(pef).peHeader();
  ..
}

void PackExecutable(std::string srcFileName, std::string outFileName)
{
  auto pef = PeLib::openPeFile(srcFileName);
  if (!pef)
  {
    std::cout << "Invalid PE File" << std::endl;
    return;
  }

  pef->readMzHeader();
  rebuildMZHeader(pef, outFileName);

  pef->readPeHeader();
  DumpPeHeaderVisitor peVisitor(outFileName);
  pef->visit(peVisitor);
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::string outFileName) :
    outFileName_(outFileName)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file, outFileName_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file, outFileName_); }
private:
  std::string outFileName_;
};

//------------------------------------------------------------------------