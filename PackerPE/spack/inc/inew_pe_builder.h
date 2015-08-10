#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "includes.h"
#include "iimport_packer.h"
#include "isections_packer.h"

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

class INewPEBuilder
{
public:
  INewPEBuilder(
    SectionsArr& newSections
    , ImportsArr& newImports
    , PeFilePtr& srcPeFile
    , const std::vector<uint8_t>& sourceFileBuff):
    newSections_(newSections)
    , newImports_(newImports)
    , srcPeFile_(srcPeFile)
    , sourceFileBuff_(sourceFileBuff)
  {}

  virtual Expected<std::vector<uint8_t>> GenerateOutputPEFile() = 0;

protected:
  SectionsArr newSections_;
  ImportsArr newImports_;
  PeFilePtr srcPeFile_;
  const std::vector<uint8_t> sourceFileBuff_;
};