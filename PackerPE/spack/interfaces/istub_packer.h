#pragma once

#include "includes.h"
#include "ipacker_base.h"
#include "isections_packer.h"
#include "stub_data.h"

class IStubPacker:
  public IPackerBase
{
public:
  IStubPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    IPackerBase(srcPEFile, PackerType::kStubPacker)
    , stubData_{}
  {}
  virtual ~IStubPacker(){};
  virtual std::vector<uint8_t> ProcessExecutable() = 0;

protected:
  stub::STUB_DATA stubData_;
};