#pragma once

#include "iloader_packer.h"

class LoaderPacker :
  public ILoaderPacker
{
public:
  LoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile);
  void ProcessExecutable(
    std::vector<uint8_t>& outFileBuffer,
    const AdditionalDataBlocksType& additionalDataBlocks);
  Expected<ErrorCode> IsReady(const std::set<PackerType>& readyPackersList) const
  {
    std::set<PackerType> dependencies{PackerType::kSectionsPacker, PackerType::kImportPacker, PackerType::kStubPacker};

    if (std::includes(readyPackersList.cbegin(), readyPackersList.cend(), dependencies.cbegin(), dependencies.cend()))
    {
      return ErrorCode::kOk;
    }

    return ErrorCode::kBusy;
  }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};