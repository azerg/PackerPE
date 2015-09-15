//
#include "loader_packer.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include "common_utils.h"

boost::filesystem::path GetLoadersPath()
{
  // load loader data
  boost::filesystem::path loaderFullPath = boost::filesystem::initial_path();
  return loaderFullPath;
}

LoaderPacker::LoaderPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile):
  ILoaderPacker(srcPEFile, GetLoadersPath())
{}

void LoaderPacker::ProcessExecutable(std::vector<uint8_t>& outFileBuffer, const AdditionalDataBlocksType& additionalDataBlocks)
{
  AdditionalDataBlocksType preallocatedBlocks;
  std::copy_if(
    additionalDataBlocks.cbegin()
    , additionalDataBlocks.cend()
    , std::back_inserter(preallocatedBlocks)
    , [](const auto& block)->auto
  {
    return block.ownerType == PackerType::kLoaderPacker;
  });

  assert(preallocatedBlocks.size() == 1);
  auto loaderBlock = preallocatedBlocks.front();

  // get type of source executable
  PlatformType platformType;
  auto sourcePEbits = srcPEFile_->getBits();
  switch (sourcePEbits)
  {
  case 32:
    platformType = PlatformType::x86;
    break;
  case 64:
    platformType = PlatformType::x64;
    break;
  default:
    break;
  }

  auto loaderInfo = std::find_if(gloadersNames.begin(), gloadersNames.end(), [&platformType](LoaderInfo& loaderInfo)->bool
  {
    return loaderInfo.platformType == platformType;
  });

  if (loaderInfo == gloadersNames.end())
  {
    throw std::runtime_error("error finding loader");
  }

  // load loader data
  boost::filesystem::path loaderFullPath = loadersStoragePath_;
  loaderFullPath /= loaderInfo->loaderName;

  boost::filesystem::ifstream loaderFile;
  loaderFile.open(loaderFullPath, std::ios::in || std::ifstream::binary);

  auto pbuf = loaderFile.rdbuf();
  // get file size
  auto size = pbuf->pubseekoff(0, loaderFile.end, loaderFile.in);
  pbuf->pubseekpos(0, loaderFile.in);

  std::vector<PeLib::byte> loaderData(size);

  // get file data
  pbuf->sgetn(reinterpret_cast<char*>(loaderData.data()), size);

  loaderFile.close();

  utils::ReplaceContainerData(outFileBuffer, loaderBlock.rawOffset, loaderData);
}

std::vector<RequiredDataBlock> LoaderPacker::GetRequiredDataBlocks() const
{
  std::vector<RequiredDataBlock> result;
  result.push_back({0x1000, packerType_});
  return result;
}