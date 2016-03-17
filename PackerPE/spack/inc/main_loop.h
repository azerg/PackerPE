#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "imain_loop.h"

class MainLoop:
  public IMainLoop
{
public:
  MainLoop(const std::string& srcFilePath, const std::string& destFilePath):
    IMainLoop(srcFilePath, destFilePath)
  {}
  MainLoop(const std::string& srcFilePath, const std::string& destFilePath, std::forward_list<IPackerBasePtr>&& packersVt) :
    IMainLoop(srcFilePath, destFilePath, std::move(packersVt))
  {}
  ErrorCode PackFile();

  void ProcessPacker(IPackerBasePtr& packer, const std::vector<uint8_t>& sourceFileBuff, std::vector<uint8_t>& outFileBuffer)
  {
    switch (packer->GetPackerType())
    {
    case PackerType::kImportPacker:
      {
        auto requiredDataBlocks = LoadRequiredDataBlocks();
        auto pImportPacker = &dynamic_cast<IImportPacker&>(*packer);
        imports_ = pImportPacker->ProcessExecutable(newSections_.additionalDataBlocks, stubData_);
      }
      break;
    case PackerType::kSectionsPacker:
      {
        auto requiredDataBlocks = LoadRequiredDataBlocks();
        auto pSectionsPacker = &dynamic_cast<ISectionsPacker&>(*packer);
        newSections_ = pSectionsPacker->ProcessExecutable(sourceFileBuff, requiredDataBlocks);
      }
      break;
    case PackerType::kStubPacker:
      {
        auto pStubPacker_ = &dynamic_cast<IStubPacker&>(*packer);
        stubData_ = pStubPacker_->ProcessExecutable();
      }
      break;
    case PackerType::kLoaderPacker:
      {
        auto pLoaderPacker = &dynamic_cast<ILoaderPacker&>(*packer);
        pLoaderPacker->ProcessExecutable(outFileBuffer, newSections_.additionalDataBlocks);
      }
      break;
    default:
      throw std::runtime_error("Unexpected packer type!");
      break;
    }
  }

private:
  bool PackerIsReady(PackerType packerType, const std::set<PackerType>& readyPackersVt) const;
  boost::optional<IPackerBasePtr> GetPacker(PackerType packerType) const noexcept;
  std::vector<RequiredDataBlock> LoadRequiredDataBlocks() const;

  ImportsArr imports_;
  std::vector<uint8_t> stubData_;
  SectionsArr newSections_;
};