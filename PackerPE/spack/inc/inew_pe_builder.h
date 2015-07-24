#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "includes.h"
#include "iimport_packer.h"
#include "isections_packer.h"

class INewPEBuilder
{
public:
  INewPEBuilder(SectionsArr& newSections, ImportsArr& newImports, std::shared_ptr<PeLib::PeFile>& srcPeFile):
    newSections_(newSections)
    , newImports_(newImports)
    , srcPeFile_(srcPeFile)
  {}

  virtual Expected<ErrorCode> GenerateOutputPEFile() = 0;

protected:
  SectionsArr newSections_;
  ImportsArr newImports_;
  std::shared_ptr<PeLib::PeFile> srcPeFile_;
};