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
private:
  bool PackerIsReady(PackerType packerType) const;
  boost::optional<IPackerBasePtr> GetPacker(PackerType packerType) const noexcept;
};