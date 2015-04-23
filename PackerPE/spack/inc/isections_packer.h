#pragma once

#include "PeLib.h"
#include "error_defines.h"

#include "expected.hpp"
#include <memory>
#include <cstdint>
#include <vector>

class ISectionsPacker
{
public:
  ISectionsPacker(std::shared_ptr<PeLib::PeFile> peFile):
    peFile_(peFile)
  {}
  virtual ~ISectionsPacker(){};

  virtual Expected<ErrorCode> ProcessExecutable(
    const std::vector<uint8_t>& sourceFileBuff
    , std::vector<uint8_t>& outFileBuff) = 0;

protected:
  std::shared_ptr<PeLib::PeFile> peFile_;
};