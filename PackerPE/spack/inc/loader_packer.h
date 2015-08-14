#pragma once

#include "includes.h"
#include "iloader_packer.h"

class LoaderPacker :
  public ILoaderPacker
{
public:
  LoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    ILoaderPacker(srcPEFile, PackerType::kStubPacker)
  {}
  virtual ~LoaderPacker() {};
  virtual void ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const STUB_DATA stubData) = 0;
};