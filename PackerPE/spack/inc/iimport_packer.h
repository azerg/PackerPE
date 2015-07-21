#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include "includes.h"
#include "expected.hpp"

#include "error_defines.h"

struct ImportEntry
{
  std::vector<PeLib::byte> old_imports;
  std::vector<PeLib::byte> new_imports;
};

typedef ImportEntry ImportsArr;

class IImportPacker
{
public:
  IImportPacker(std::shared_ptr<PeLib::PeFile> srcPEFile):
    srcPEFile_(srcPEFile)
  {}
  virtual ~IImportPacker(){};
  virtual uint32_t GetRequiredSpaceSize() const = 0;
  virtual ImportsArr ProcessExecutable(PeLib::dword importTableRVA) = 0;
  /*! \brief Validates source executable
  * This function is used to validate source PE-file, checking
  * whether its ready for applying definite packer's part.
  * Goal of this proc is to make all packer's pieces to be completely
  * independent.
  */
  virtual Expected<ErrorCode> IsReady() const = 0;

protected:
  std::shared_ptr<PeLib::PeFile> srcPEFile_;
};