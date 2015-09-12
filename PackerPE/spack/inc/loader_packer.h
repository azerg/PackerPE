#pragma once

#include "includes.h"
#include "iloader_packer.h"

class LoaderPacker :
  public ILoaderPacker
{
public:
  LoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile);
  LoaderInfo ProcessExecutable(boost::filesystem::path loadersStoragePath);
  Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};