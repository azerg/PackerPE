#pragma once

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
#pragma warning( pop ) 

#include "isections_packer.h"

class SectionsPacker: ISectionsPacker
{
public:
  SectionsPacker(std::shared_ptr<PeLib::PeFile> peFile):
    ISectionsPacker(peFile)
  {}
  ~SectionsPacker(){};

  Expected<ErrorCode> ProcessExecutable(
    const std::vector<uint8_t>& sourceFileBuff
    , std::vector<uint8_t>& outFileBuff
    , std::shared_ptr<PeLib::PeFile> peFileOut);
};