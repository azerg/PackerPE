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

class DumpSectionsVisitor : public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(const std::vector<uint8_t>& sourceFileBuff, ImportsArr& importOut) :
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
  return ImportsArr();
}