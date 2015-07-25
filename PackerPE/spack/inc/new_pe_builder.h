#pragma once

#include "includes.h"
#include "inew_pe_builder.h"

class NewPEBuilder: INewPEBuilder
{
public:
  NewPEBuilder(
    SectionsArr& newSections
    , ImportsArr& newImports
    , PeFilePtr& srcPeFile
    , const std::vector<uint8_t>& sourceFileBuff):
    INewPEBuilder(newSections, newImports, srcPeFile, sourceFileBuff)
  {}

  Expected<std::vector<uint8_t>> GenerateOutputPEFile();
};