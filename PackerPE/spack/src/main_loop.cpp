#pragma once

#include "main_loop.h"

bool MainLoop::PackerIsReady(PackerType packerType) const
{
  auto existentPacker = GetPacker(packerType);
  if (!existentPacker.is_initialized())
  {
    throw std::runtime_error("Required packer is not exists");
  }

  auto isReady = (*existentPacker)->IsReady(readyPackersVt_);

  return isReady.get() == ErrorCode::kOk;
}

ErrorCode MainLoop::PackFile()
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
