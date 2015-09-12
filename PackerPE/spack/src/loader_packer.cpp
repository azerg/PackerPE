//
#include "loader_packer.h"
#include <boost/filesystem/fstream.hpp>

LoaderPacker::LoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
  ILoaderPacker(srcPEFile)
{}

LoaderInfo LoaderPacker::ProcessExecutable(boost::filesystem::path loadersStoragePath)
{
  // load loader data
  boost::filesystem::path loaderFullPath;
  for (auto loaderName : gloadersNames)
  {
    boost::filesystem::ifstream loaderFile;
    loaderFullPath /= loaderName;
    loaderFile.open(loaderFullPath, std::ios::in);

    auto fileBuff = loaderFile.rdbuf();
  }
  return LoaderInfo();
}

std::vector<RequiredDataBlock> LoaderPacker::GetRequiredDataBlocks() const
{
  return std::vector<RequiredDataBlock>();
}