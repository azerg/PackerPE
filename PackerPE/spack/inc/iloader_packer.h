#pragma once

#include <memory>
#include <cstdint>
#include <list>
#include "includes.h"
#include "ipacker_base.h"

class ILoaderPacker:
  public IPackerBase
{
public:
  ILoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    IPackerBase(srcPEFile, PackerType::kStubPacker)
  {}
  virtual ~ILoaderPacker(){};
  virtual void ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const STUB_DATA stubData) = 0;
};