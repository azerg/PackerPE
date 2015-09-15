#pragma once

#include <vector>
#include "includes.h"
#include "ipacker_base.h"
#include "isections_packer.h"
#include "boost/filesystem/path.hpp"

enum class PlatformType
{
  x86,
  x64
};

struct LoaderInfo
{
  std::string loaderName;
  PlatformType platformType;
};

static std::vector<LoaderInfo> gloadersNames{{"loader_x86.stub", PlatformType::x86}, {"loader_x86.stub", PlatformType::x64}}; // dup data here just for testing of two loaders :D

class ILoaderPacker:
  public IPackerBase
{
public:
  ILoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile, boost::filesystem::path loadersStoragePath):
    loadersStoragePath_(loadersStoragePath)
    , IPackerBase(srcPEFile, PackerType::kLoaderPacker)
  {}
  virtual ~ILoaderPacker(){};
  virtual void ProcessExecutable(std::vector<uint8_t>& outFileBuffer, const AdditionalDataBlocksType& additionalDataBlocks) = 0;

protected:
  boost::filesystem::path loadersStoragePath_;
};