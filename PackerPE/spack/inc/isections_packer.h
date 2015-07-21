#pragma once

#include <memory>
#include <cstdint>
#include <list>
#include "expected.hpp"
#include "includes.h"
#include "error_defines.h"

enum class PackerTypes
{
  kImportPacker,
  kSectionsPacker,
};

struct RequiredDataBlock
{
  uint32_t size;
  PackerTypes ownerType;
};

struct DataBlock
{
  uint32_t offset;
  uint32_t size;
  PackerTypes ownerType;
};

struct Section
{
  PeLib::PELIB_IMAGE_SECTION_HEADER newHeader;
  PeLib::PELIB_IMAGE_SECTION_HEADER originalHeader;
  std::vector<uint8_t> data;
  std::vector<DataBlock> additionalDataBlocks;
};

typedef std::list<Section> SectionsArr;

class ISectionsPacker
{
public:
  ISectionsPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    srcPEFile_(srcPEFile)
  {}
  virtual ~ISectionsPacker(){};
  virtual SectionsArr ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const std::vector<RequiredDataBlock> additionalSizeRequest) = 0;
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