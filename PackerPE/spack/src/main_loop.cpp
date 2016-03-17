#pragma once

#include "main_loop.h"
#include "file_utils.h"

bool MainLoop::PackerIsReady(PackerType packerType, const std::set<PackerType>& readyPackersVt) const
{
  auto existentPacker = GetPacker(packerType);
  if (!existentPacker.is_initialized())
  {
    throw std::runtime_error("Required packer is not exists");
  }

  auto isReady = (*existentPacker)->IsReady(readyPackersVt);

  return isReady.get() == ErrorCode::kOk;
}

ErrorCode MainLoop::PackFile()
{
  std::set<PackerType> readyPackersVt;

  const auto& sourceFileBuff = file_utils::readFile(srcFilePath_.c_str());
  std::vector<uint8_t> outFileBuffer;

  bool bNotAllParsersProcessed;
  do
  {
    bNotAllParsersProcessed = false;

    for (auto& packer : packersVt_)
    {
      if (PackerIsReady(packer->GetPackerType(), readyPackersVt))
      {
        ProcessPacker(packer, sourceFileBuff, outFileBuffer);
        readyPackersVt.insert(packer->GetPackerType());
      }
      else
      {
        bNotAllParsersProcessed = true;
      }
    }

    // dummy check for testing /// todo(azerg): remove me plz
    static int antiInfiniteLoop{};
    if (antiInfiniteLoop > 100)
    {
      throw std::runtime_error("infinite mainloop detected");
    }
    ++antiInfiniteLoop;


  } while (bNotAllParsersProcessed);

  return ErrorCode();
}

boost::optional<IPackerBasePtr> MainLoop::GetPacker(PackerType packerType) const noexcept
{
  for (const auto& packer : packersVt_)
  {
    if (packer->GetPackerType() == packerType)
    {
      return packer;
    }
  }
  return boost::none;
}

std::vector<RequiredDataBlock> MainLoop::LoadRequiredDataBlocks() const
{
  std::vector<RequiredDataBlock> requiredDataBlocks;
  for (const auto& packer : packersVt_)
  {
    auto singlePackerRequiredDataBlocks = packer->GetRequiredDataBlocks();
    std::move(
      std::begin(singlePackerRequiredDataBlocks),
      std::end(singlePackerRequiredDataBlocks),
      std::back_inserter(requiredDataBlocks));
  }
  return requiredDataBlocks;
}
