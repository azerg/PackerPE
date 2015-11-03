#pragma once

#include "main_loop.h"

bool MainLoop::PackerIsReady(PackerType packerType, const std::forward_list<PackerType>& readyPackersVt) const
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
  auto requiredDataBlocks = LoadRequiredDataBlocks();

  std::forward_list<PackerType> readyPackersVt;

  for (const auto& packer : packersVt_)
  {
    if (PackerIsReady(packer->GetPackerType(), readyPackersVt))
    {
      //
    }
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
