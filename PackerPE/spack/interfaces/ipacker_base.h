#pragma once

#include "includes.h"
#include <set>
#include <vector>

enum class PackerType
{
  kImportPacker,
  kSectionsPacker,
  kStubPacker,
  kLoaderPacker,
};

/*!
* Every data block might have some requirements, e.g. it have to be allocated
* in executable section, whatever..
*/
enum class DataBlockRequirements
{
  kNoRequirements,
  kExecutableSection, // data might be located in executable section
};

struct RequiredDataBlock
{
  RequiredDataBlock(RequiredDataBlock&& right) = default;
  RequiredDataBlock(const RequiredDataBlock&) = default;
  uint32_t size;
  PackerType ownerType;
  int32_t packerParam; // optinal value is set by packer and used in packer
  std::vector<DataBlockRequirements> requirements;
};

struct DataBlock
{
  uint32_t rawOffset;
  uint32_t virtualOffset; // uint64_t ?
  uint32_t size;
  PackerType ownerType;
  int32_t packerParam;
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
  virtual Expected<ErrorCode> IsReady(const std::set<PackerType>& readyPackersList) const = 0;
  PackerType GetPackerType() const noexcept { return packerType_; }
protected:
  std::shared_ptr<PeLib::PeFile> srcPEFile_;
  PackerType packerType_;
};

typedef std::shared_ptr<IPackerBase> IPackerBasePtr;