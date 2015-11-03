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

int32_t GetStubDataOffset(const AdditionalDataBlocksType& additionalDataBlocks)
{
  auto stubBlock = utils::GetSingleAdditionalBlock(additionalDataBlocks, PackerType::kStubPacker);
  auto loaderBlock = utils::GetSingleAdditionalBlock(additionalDataBlocks, PackerType::kLoaderPacker);

  return loaderBlock.virtualOffset - stubBlock.virtualOffset;
}

uint32_t GetRawDataOffsetInStubData(std::vector<PeLib::byte>& loaderBuff)
{
  // looking for marker inside loaderBuff. (O(n) but doesnt matters yet)
  auto loaderSize = loaderBuff.size();
  for (auto loaderBuffIdx = 0; loaderBuffIdx <= loaderSize; ++loaderBuffIdx)
  {
    auto* byte = &loaderBuff.at(loaderBuffIdx);
    if (*byte == 0xc1)
    {
      auto tmp = *reinterpret_cast<uint64_t*>(byte);
      if (*reinterpret_cast<uint64_t*>(byte) == 0x9090d0d0c1d0d0c1) // raw data rcl-based magic
      {
        return loaderBuffIdx;
      }
    }
  }

  throw std::runtime_error("Invalid loader - cant find magic");
}

// this function is used to save offset to stubData somewhere in loader :D
void ModifyLoaderWithStubDataInfo(std::vector<PeLib::byte>& inOutLoaderBuff, const AdditionalDataBlocksType& additionalDataBlocks)
{
  auto stubDataOffset = GetStubDataOffset(additionalDataBlocks);

  const auto loaderCaveOffset = GetRawDataOffsetInStubData(inOutLoaderBuff);

  // add caveOffset to stubDataOffset to get stubData easily from loader
  stubDataOffset += loaderCaveOffset;

  *reinterpret_cast<int32_t*>(&inOutLoaderBuff.at(loaderCaveOffset)) = stubDataOffset;
}

void LoaderPacker::ProcessExecutable(
  std::vector<uint8_t>& outFileBuffer,
  const AdditionalDataBlocksType& additionalDataBlocks)
{
  auto loaderBlock = utils::GetSingleAdditionalBlock(additionalDataBlocks, PackerType::kLoaderPacker);

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

  // get stub data offset to update loader code with pointer to stub data ( link stubData to loader )
  ModifyLoaderWithStubDataInfo(loaderData, additionalDataBlocks);

  utils::ReplaceContainerData(outFileBuffer, loaderBlock.rawOffset, loaderData);
}

std::vector<RequiredDataBlock> LoaderPacker::GetRequiredDataBlocks() const
{
  std::vector<RequiredDataBlock> result;
  result.push_back({0x1000, packerType_});
  return result;
}