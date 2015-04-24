#pragma once

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
#pragma warning( pop ) 

#include "isections_packer.h"

class SectionsPacker: ISectionsPacker
{
public:
  Expected<ErrorCode> ProcessExecutable(std::string& srcFileName, std::string& outFileName);
};