#pragma once

#include "PeLib.h"
#include "error_defines.h"

#include "expected.hpp"
#include <memory>
#include <cstdint>

class IImportPacker
{
public:
  IImportPacker(std::shared_ptr<PeLib::PeFile> srcPEFile):
    srcPEFile_(srcPEFile)
  {}
  virtual ~IImportPacker(){};
  virtual Expected<ErrorCode> ProcessExecutable(const std::string& outFileName) = 0;
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