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
  virtual ~ISectionsPacker(){};
  virtual Expected<ErrorCode> ProcessExecutable(std::string& srcFileName, std::string& outFileName) = 0;
};