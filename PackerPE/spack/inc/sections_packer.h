#pragma once

#include "isections_packer.h"

class SectionsPacker: ISectionsPacker
{
public:
  SectionsPacker(std::weak_ptr<PeLib::PeFile> peFile):
    ISectionsPacker(peFile)
  {}
  virtual ~SectionsPacker();

  Expected<ErrorCode> ProcessExecutable(std::weak_ptr<PeLib::PeFile> peFileOut)
  {
    return ErrorCode::ERROR_SUCC;
  }
};