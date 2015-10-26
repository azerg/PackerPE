#pragma once

#include <list>
#include "includes.h"
#include "ipacker_base.h"

struct Section
{
  PeLib::PELIB_IMAGE_SECTION_HEADER newHeader;
  PeLib::PELIB_IMAGE_SECTION_HEADER originalHeader;
  std::vector<uint8_t> data;
};

struct SectionsArr
{
  std::list<Section> sections;
  std::vector<DataBlock> additionalDataBlocks; // blocks allocated in packed file for other packers
};

class ISectionsPacker:
  public IPackerBase
{
public:
  ISectionsPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
    IPackerBase(srcPEFile, PackerType::kSectionsPacker)
  {}
  virtual ~ISectionsPacker(){};
  virtual SectionsArr ProcessExecutable(
    const std::vector<uint8_t>& sourceFileBuff
    , const std::vector<RequiredDataBlock> additionalSizeRequest) = 0;
};


typedef decltype(SectionsArr::additionalDataBlocks) AdditionalDataBlocksType;