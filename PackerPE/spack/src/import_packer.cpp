//
#include "import_packer.h"

template <int bits>
PeLib::ImportDirectory<bits> GenerateDefaultImports()
{
  PeLib::ImportDirectory<bits> newImp;

  newImp.addFunction("kernel32.dll", "LoadLibraryA");
  newImp.addFunction("kernel32.dll", "GetProcAddress");

  return newImp;
}

template<int bits>
void dumpImportDirectory(
  PeLib::PeFile& peFile
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
  DumpImportsVisitor(ImportsArr& importOut, PeLib::dword newImportTableRVA):
    importOut_(importOut)
    , newImportTableRVA_(newImportTableRVA)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    dumpImportDirectory<32>(file, importOut_, newImportTableRVA_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    dumpImportDirectory<64>(file, importOut_, newImportTableRVA_);
  }
private:
  ImportsArr& importOut_;
  PeLib::dword newImportTableRVA_;
};

ImportsArr ImportPacker::ProcessExecutable(const AdditionalDataBlocksType& additionalDataBlocks)
{
  // pick last section VA to use it as new imports VA
  auto additionalImportsBlock = std::find_if(
    additionalDataBlocks.begin()
    , additionalDataBlocks.end()
    , [&](auto& valIt)
  {
    return valIt.ownerType == PackerType::kImportPacker &&
      valIt.packerParam == ( int32_t )ImportBlockTypes::kNewImportData;
  });

  auto newImportTableRVA = 0;

  if (additionalImportsBlock != additionalDataBlocks.end())
  {
    newImportTableRVA = additionalImportsBlock->virtualOffset;
  }
  else
  {
    throw std::runtime_error("unexpected error - missing new import table VA.");
  }

  ImportsArr importsData;
  DumpImportsVisitor importsVisitor(importsData, newImportTableRVA);
  srcPEFile_->visit(importsVisitor);

  return importsData;
}

std::vector<RequiredDataBlock> ImportPacker::GetRequiredDataBlocks() const
{
  ImportsArr importsData;

  DumpImportsVisitor importsVisitor(importsData, 0);
  srcPEFile_->visit(importsVisitor);

  std::vector<RequiredDataBlock> result;
  result.push_back({importsData.new_imports.size(), packerType_, (int32_t)ImportBlockTypes::kNewImportData});
  result.push_back({importsData.old_imports.size(), packerType_, (int32_t)ImportBlockTypes::kOldImportData});
  return result;
}