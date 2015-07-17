#pragma once

#include <memory>
#include <cstdint>

#include "expected.hpp"

#include "PeLib.h"
#include "error_defines.h"

template <int bits>
struct ImportEntry
{
  PeLib::PELIB_IMAGE_IMPORT_DIRECTORY<bits> old_imports;
  PeLib::PELIB_IMAGE_IMPORT_DIRECTORY<bits> new_imports;
};

typedef std::vector<ImportEntry<64>> ImportsArr;

class IImportPacker
{
public:
  IImportPacker(std::shared_ptr<PeLib::PeFile> srcPEFile):
    srcPEFile_(srcPEFile)
  {}
  virtual ~IImportPacker(){};
  virtual ImportsArr ProcessExecutable() = 0;
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