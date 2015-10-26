#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include "imain_loop.h"

class MainLoop:
  public IMainLoop
{
public:
  bool PackerIsReady(PackerType packerType) const
  {
    auto existentPacker = GetPacker(packerType);
    if (!existentPacker.is_initialized())
    {
      throw std::runtime_error("Required packer is not exists");
    }

    auto isReady = (*existentPacker)->IsReady();

    return isReady.get() == ErrorCode::ERROR_SUCC;
  }
private:
  boost::optional<IPackerBasePtr> GetPacker(PackerType packerType) const
  {
    for (const auto& packer : packersVt_)
    {
      if (packer->GetPackerType() == packerType)
      {
        return packer;
      }
    }
    boost::optional<IPackerBasePtr>();
  }

};