#pragma once

#include "includes.h"

enum class PackerType
{
  kImportPacker,
  kSectionsPacker,
  kStubPacker,
};

struct RequiredDataBlock
{
  uint32_t size;
  PackerType ownerType;
};

struct DataBlock
{
  uint32_t rawOffset;
  uint32_t virtualOffset; // uint64_t ?
  uint32_t size;
  PackerType ownerType;
};

// base class for all packers
class IPackerBase
{
public:
  IPackerBase(std::shared_ptr<PeLib::PeFile> srcPEFile, PackerType packerType):
    srcPEFile_(srcPEFile)
    , packerType_(packerType)
  {}
  virtual ~IPackerBase(){};
  virtual std::vector<RequiredDataBlock> GetRequiredDataBlocks() const = 0;
  /*! \brief Validates source executable
  * This function is used to validate source PE-file, checking
  * whether its ready for applying definite packer's part.
  * Goal of this proc is to make all packer's pieces to be completely
  * independent.
  */
  virtual Expected<ErrorCode> IsReady() const = 0;
protected:
  std::shared_ptr<PeLib::PeFile> srcPEFile_;
  PackerType packerType_;
};