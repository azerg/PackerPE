#pragma once

#include "includes.h"
#include "iimport_packer.h"

enum class ImportBlockTypes : decltype(RequiredDataBlock::packerParam)
{
  kOldImportData,
  kNewImportData,
};

class ImportPacker : IImportPacker
{
public:
  ImportPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IImportPacker(srcPEFile)
  {}
  ImportsArr ProcessExecutable(const decltype(SectionsArr::additionalDataBlocks)& additionalDataBlocks);
  Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};