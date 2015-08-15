#pragma once

#include <memory>
#include <cstdint>
#include <list>
#include "includes.h"
#include "ipacker_base.h"
#include "stub_data.h"

class IStubPacker:
  public IPackerBase
{
public:
  IStubPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    IPackerBase(srcPEFile, PackerType::kStubPacker)
  {}
  virtual ~IStubPacker(){};
  virtual void ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const stub::STUB_DATA stubData) = 0;
};