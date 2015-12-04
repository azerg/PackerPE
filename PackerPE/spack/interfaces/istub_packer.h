#pragma once

#include <list>
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
  virtual void ProcessExecutable(
    std::vector<uint8_t>& outFileBuffer
    , const AdditionalDataBlocksType& additionalDataBlocks) = 0;
  virtual stub::STUB_DATA& GetStubData(){ return stubData_; }

protected:
  stub::STUB_DATA stubData_;
};