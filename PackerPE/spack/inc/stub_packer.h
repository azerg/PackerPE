#pragma once

#include "istub_packer.h"

class StubPacker :
  public IStubPacker
{
public:
  StubPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IStubPacker(srcPEFile)
  {}
  virtual ~StubPacker() {};
  virtual void ProcessExecutable(
    std::vector<uint8_t>& sourceFileBuff
    , const AdditionalDataBlocksType& additionalDataBlocks);
  Expected<ErrorCode> IsReady(const std::forward_list<PackerType>& readyPackersList) const { return ErrorCode::kOk; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const
  {
    std::vector<RequiredDataBlock> result;
    result.push_back({sizeof(stub::STUB_DATA), PackerType::kStubPacker});
    return result;
  }
private:
  std::vector<uint8_t> InititalizeStub(
    std::vector<uint8_t>& sourceFileBuff
    , const AdditionalDataBlocksType& additionalDataBlocks);
};