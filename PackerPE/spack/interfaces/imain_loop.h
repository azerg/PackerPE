#pragma once

#include "includes.h"
#include "ipacker_base.h"
#include <vector>

class IMainLoop
{
public:
  IMainLoop(const std::string& srcFilePath, const std::string& destFilePath) :
    srcFilePath_(srcFilePath),
    destFilePath_(destFilePath)
  {}
  IMainLoop(const std::string& srcFilePath, const std::string& destFilePath, std::vector<IPackerBasePtr>&& packersVt) :
    IMainLoop(srcFilePath, destFilePath)
  {
    packersVt_ = std::move(packersVt);
  }
  virtual bool PackerIsReady(PackerType packerType) const = 0;
  size_t AddPacker(IPackerBasePtr&& packer)
  {
    packersVt_.push_back(packer);
    return packersVt_.size();
  }
protected:
  std::vector<IPackerBasePtr> packersVt_;
  const std::string srcFilePath_;
  const std::string destFilePath_;
};