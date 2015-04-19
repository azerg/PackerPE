#pragma once

#include "PeLib.h"
#include "error_defines.h"

#include "expected.hpp"
#include <memory>

class ISectionsPacker
{
public:
  ISectionsPacker(std::weak_ptr<PeLib::PeFile> peFile):
    peFile_(peFile)
  {}
  virtual ~ISectionsPacker();

  virtual Expected<ErrorCode> ProcessExecutable(std::weak_ptr<PeLib::PeFile> peFileOut) = 0;

protected:
  std::weak_ptr<PeLib::PeFile> peFile_;
};