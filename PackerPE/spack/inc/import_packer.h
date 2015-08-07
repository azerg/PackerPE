#pragma once

#include "includes.h"
#include "iimport_packer.h"

class ImportPacker : IImportPacker
{
public:
  ImportPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IImportPacker(srcPEFile)
  {}
  uint32_t GetRequiredSpaceSize() const;
  ImportsArr ProcessExecutable(PeLib::dword importTableRVA);
  Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const;
};