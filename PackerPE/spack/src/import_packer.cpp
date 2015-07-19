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
void dumpImportDirectory(
  PeLib::PeFile& peFile
  , const std::vector<uint8_t>& sourceFileBuff
  , ImportsArr& importOut
  , PeLib::dword newImportTableRVA)
{
  if (peFile.readImportDirectory() != PeLib::NO_ERROR)
  {
    return;
  }

  auto& imp = static_cast<PeLib::PeFileT<bits>&>(peFile).impDir();
  imp.rebuild(importOut.old_imports, 0);

  // filling new import table
  auto newImp = GenerateDefaultImports<bits>();
  newImp.rebuild(importOut.new_imports, newImportTableRVA);
}

class DumpImportsVisitor : public PeLib::PeFileVisitor
{
public:
  DumpImportsVisitor(const std::vector<uint8_t>& sourceFileBuff, ImportsArr& importOut, PeLib::dword newImportTableRVA):
    sourceFileBuff_(sourceFileBuff)
    , importOut_(importOut)
    , newImportTableRVA_(newImportTableRVA)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    dumpImportDirectory<32>(file, sourceFileBuff_, importOut_, newImportTableRVA_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    dumpImportDirectory<64>(file, sourceFileBuff_, importOut_, newImportTableRVA_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  ImportsArr& importOut_;
  PeLib::dword newImportTableRVA_;
};

ImportsArr ImportPacker::ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, PeLib::dword newImportTableRVA)
{
  ImportsArr importsData;

  DumpImportsVisitor importsVisitor(sourceFileBuff, importsData, newImportTableRVA);
  srcPEFile_->visit(importsVisitor);

  return importsData;
}