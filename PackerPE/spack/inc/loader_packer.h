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
  Expected<ErrorCode> IsReady(const std::forward_list<PackerType>& readyPackersList) const { return ErrorCode::kOk; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};