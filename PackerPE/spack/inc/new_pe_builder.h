#pragma once

#include "includes.h"
#include "inew_pe_builder.h"

class NewPEBuilder: INewPEBuilder
{
public:
  NewPEBuilder(SectionsArr& newSections, ImportsArr& newImports, std::shared_ptr<PeLib::PeFile>& srcPeFile):
    INewPEBuilder(newSections, newImports, srcPeFile)
  {}

  Expected<ErrorCode> GenerateOutputPEFile();
};