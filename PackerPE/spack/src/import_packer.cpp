//
#include "import_packer.h"

template <int bits>
PeLib::ImportDirectory<bits> GenerateDefaultImports()
{
  PeLib::ImportDirectory<bits> newImp;

  newImp.addFunction("kernel32.dll", "CreateFileW");
  newImp.addFunction("kernel32.dll", "CreateFileA");

  return newImp;
}

template<int bits>
void dumpImportDirectory(PeLib::PeFile& pef, const std::vector<uint8_t>& sourceFileBuff, ImportsArr& importOut)
{
  if (pef.readImportDirectory() != PeLib::NO_ERROR)
  {
    return;
  }

  auto& imp = static_cast<PeLib::PeFileT<bits>&>(pef).impDir();
  imp.rebuild(importOut.old_imports, 0);

  // filling new import table
  auto newImp = GenerateDefaultImports<bits>();
  newImp.rebuild(importOut.new_imports, 0);
}

class DumpImportsVisitor : public PeLib::PeFileVisitor
{
public:
  DumpImportsVisitor(const std::vector<uint8_t>& sourceFileBuff, ImportsArr& importOut) :
    sourceFileBuff_(sourceFileBuff)
    , importOut_(importOut)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    dumpImportDirectory<32>(file, sourceFileBuff_, importOut_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    dumpImportDirectory<64>(file, sourceFileBuff_, importOut_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  ImportsArr& importOut_;
};

ImportsArr ImportPacker::ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff)
{
  ImportsArr importsData;

  DumpImportsVisitor importsVisitor(sourceFileBuff, importsData);
  srcPEFile_->visit(importsVisitor);

  return importsData;
}