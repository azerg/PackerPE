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
  ImportsArr ProcessExecutable(const AdditionalDataBlocksType& additionalDataBlocks, std::vector<uint8_t>& stubDataToUpdate);
  Expected<ErrorCode> IsReady(const std::set<PackerType>& readyPackersList) const
  {
    if (readyPackersList.find(PackerType::kSectionsPacker) != readyPackersList.cend())
    {
      return ErrorCode::kOk;
    }

    return ErrorCode::kBusy;
  }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};