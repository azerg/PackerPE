#pragma once

#include "includes.h"
#include "iimport_packer.h"

enum class ImportBlockTypes : decltype(RequiredDataBlock::packerParam)
{
  kOldImportData,
  kNewImportData,
};

class ImportPacker :
  public IImportPacker
{
public:
  ImportPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IImportPacker(srcPEFile)
  {}
  ImportsArr ProcessExecutable(const AdditionalDataBlocksType& additionalDataBlocks, stub::STUB_DATA& stubDataToUpdate);
  Expected<ErrorCode> IsReady(const std::forward_list<PackerType>& readyPackersList) const { return ErrorCode::kOk; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};