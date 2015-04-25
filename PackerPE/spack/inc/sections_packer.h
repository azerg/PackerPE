#pragma once

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
#pragma warning( pop ) 

#include "isections_packer.h"

class SectionsPacker: ISectionsPacker
{
public:
  SectionsPacker(const std::string& srcFileName):
    ISectionsPacker(srcFileName)
  {}
  virtual Expected<ErrorCode> ProcessExecutable(const std::string& outFileName);
  virtual Expected<ErrorCode> IsReady() const;
private:
  uint32_t GetSectionsOffset() const;
};