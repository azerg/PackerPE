#pragma once

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
#pragma warning( pop ) 

#include "isections_packer.h"

class SectionsPacker: ISectionsPacker
{
public:
  SectionsPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    ISectionsPacker(srcPEFile)
  {}
  virtual SectionsArr ProcessExecutable();
  virtual Expected<ErrorCode> IsReady() const;
private:
  uint32_t GetSectionsOffset() const;
};