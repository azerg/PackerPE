#pragma once

#include "includes.h"
#include "ipacker_base.h"
#include <vector>

class IMainLoop
{
public:
  IMainLoop() {}
  IMainLoop(std::vector<IPackerBasePtr>&& packersVt) :
    packersVt_(packersVt) {}
  virtual bool PackerIsReady(PackerType packerType) const = 0;
  size_t AddPacker(IPackerBasePtr&& packer)
  {
    packersVt_.push_back(packer);
  }
protected:
  std::vector<IPackerBasePtr> packersVt_;
};