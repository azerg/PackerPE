#pragma once

#include "import_packer.h"

#include "file_utils.h"

template<int bits>
void dumpImportDirectory(PeLib::PeFile& pef, const std::vector<uint8_t>& sourceFileBuff, ImportsArr& importOut)
{
  if (!pef.readImportDirectory())
  {
    return;
  }

  const PeLib::ImportDirectory<bits>& imp = static_cast<PeLib::PeFileT<bits>&>(pef).impDir();

  auto ceImportedDLLs = imp.getNumberOfFiles(PeLib::OLDDIR);
  importOut.reserve(ceImportedDLLs * PeLib::PELIB_IMAGE_IMPORT_DESCRIPTOR::size());

  std::vector<PeLib::byte> importsBuff;
  imp.rebuild(importsBuff, 0);

  for (unsigned int i = 0; i < imp.getNumberOfFiles(PeLib::OLDDIR); i++)
  {
    PeLib::PELIB_IMAGE_IMPORT_DESCRIPTOR oldImportDLLEntry;
    /*
    oldImportDLLEntry.FirstThunk = imp.getFirstThunk(i, PeLib::OLDDIR);
    oldImportDLLEntry.ForwarderChain = imp.getForwarderChain(i, PeLib::OLDDIR);
    oldImportDLLEntry.Name = imp.getFileName(i, PeLib::OLDDIR);
    oldImportDLLEntry.OriginalFirstThunk = imp.getOriginalFirstThunk(i, PeLib::OLDDIR);
    oldImportDLLEntry.TimeDateStamp = */
  }

  /*
  for (unsigned int i = 0; i<imp.getNumberOfFiles(PeLib::OLDDIR); i++)
  {
    dump(formatOutput("DLL Name", imp.getFileName(i, PeLib::OLDDIR)));
    dump(formatOutput("OriginalFirstThunk", toString(imp.getOriginalFirstThunk(i, PeLib::OLDDIR)), "    "));
    dump(formatOutput("TimeDateStamp", toString(imp.getTimeDateStamp(i, PeLib::OLDDIR)), "    "));
    dump(formatOutput("ForwarderChain", toString(imp.getForwarderChain(i, PeLib::OLDDIR)), "    "));
    dump(formatOutput("Name", toString(imp.getRvaOfName(i, PeLib::OLDDIR)), "    "));
    dump(formatOutput("FirstThunk", toString(imp.getFirstThunk(i, PeLib::OLDDIR)), "    "));
    dump("");

    for (unsigned int j = 0; j<imp.getNumberOfFunctions(i, PeLib::OLDDIR); j++)
    {
      dump(formatOutput("Function Name", imp.getFunctionName(i, j, PeLib::OLDDIR), "    "));
      dump(formatOutput("Hint", toString(imp.getFunctionHint(i, j, PeLib::OLDDIR)), "        "));
      dump(formatOutput("First Thunk", toString(imp.getFirstThunk(i, j, PeLib::OLDDIR)), "        "));
      dump(formatOutput("Original First Thunk", toString(imp.getOriginalFirstThunk(i, j, PeLib::OLDDIR)), "        "));
      dump("");
    }

    dump("");
  }
  */
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

ImportsArr ImportPacker::ProcessExecutable()
{
  auto sourceFileBuff = file_utils::readFile(srcPEFile_->getFileName());

  ImportsArr importsData;

  DumpImportsVisitor importsVisitor(sourceFileBuff, importsData);
  srcPEFile_->visit(importsVisitor);

  return importsData;
}