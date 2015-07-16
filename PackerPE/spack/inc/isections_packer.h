#pragma once

#include "PeLib.h"
#include "error_defines.h"

#include "expected.hpp"
#include <memory>
#include <cstdint>
#include <list>

struct Section
{
  PeLib::PELIB_IMAGE_SECTION_HEADER new_header;
  PeLib::PELIB_IMAGE_SECTION_HEADER original_header;
  std::vector<uint8_t> data;
};

typedef std::list<Section> SectionsArr;

class ISectionsPacker
{
public:
  ISectionsPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    srcPEFile_(srcPEFile)
  {}
  virtual ~ISectionsPacker(){};
  virtual SectionsArr ProcessExecutable() = 0;
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