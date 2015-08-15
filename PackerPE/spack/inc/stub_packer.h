#pragma once

#include "includes.h"
#include "istub_packer.h"

class StubPacker :
  public IStubPacker
{
public:
  StubPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IStubPacker(srcPEFile)
  {}
  virtual ~StubPacker() {};
  virtual void ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const stub::STUB_DATA stubData);
  Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const
  {
    std::vector<RequiredDataBlock> result;
    result.push_back({sizeof(stub::STUB_DATA), PackerType::kStubPacker});
    return result;
  }
};