#pragma once

#include "includes.h"
#include "iimport_packer.h"

class ImportPacker : IImportPacker
{
public:
  ImportPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IImportPacker(srcPEFile)
  {}
  virtual ImportsArr ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, PeLib::dword importTableRVA);
  virtual Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
private:
  uint32_t GetSectionsOffset() const;
};