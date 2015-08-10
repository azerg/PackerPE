#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "includes.h"
#include "ipacker_base.h"

struct ImportEntry
{
  ImportEntry(){}
  ImportEntry(ImportEntry&&) = default;
  ImportEntry(ImportEntry&) = default;
  ImportEntry& operator=(ImportEntry&&) = default;
  std::vector<PeLib::byte> old_imports;
  std::vector<PeLib::byte> new_imports;
};

typedef ImportEntry ImportsArr;

class IImportPacker:
  public IPackerBase
{
public:
  IImportPacker(std::shared_ptr<PeLib::PeFile> srcPEFile):
    IPackerBase(srcPEFile, PackerType::kImportPacker)
  {}
  virtual ~IImportPacker(){};
  virtual ImportsArr ProcessExecutable(PeLib::dword importTableRVA) = 0;
};