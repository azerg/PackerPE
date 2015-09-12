#pragma once

#include <vector>
#include "includes.h"
#include "ipacker_base.h"
#include "boost/filesystem/path.hpp"

static std::vector<std::string> gloadersNames{"loader_x86.stub", "loader_x86.stub"}; // dup data here just for testing of two loaders :D

struct LoaderInfo
{
  LoaderInfo(){}
  LoaderInfo(LoaderInfo&&) = default;
  LoaderInfo(LoaderInfo&) = default;
  LoaderInfo& operator=(LoaderInfo&&) = default;
  std::vector<PeLib::byte> loader_data;
};

class ILoaderPacker:
  public IPackerBase
{
public:
  ILoaderPacker(std::shared_ptr<PeLib::PeFile> srcPEFile):
    IPackerBase(srcPEFile, PackerType::kLoaderPacker)
  {}
  virtual ~ILoaderPacker(){};
  virtual LoaderInfo ProcessExecutable(boost::filesystem::path loadersStoragePath) = 0;
};