#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <boost/spirit/home/support/container.hpp>

namespace utils
{
  template <class OutFileBufferType, class OffsetType, class NewDataContainerType>
  void ReplaceContainerData(OutFileBufferType& outFileBuffer, OffsetType rawOffset, NewDataContainerType& new_imports)
  {
    // removing preallocated buffer
    outFileBuffer.erase(
      outFileBuffer.begin() + rawOffset
      , outFileBuffer.begin() + rawOffset + new_imports.size());
    // inserting new import data
    outFileBuffer.insert(
      outFileBuffer.begin() + rawOffset
      , new_imports.cbegin(), new_imports.cend());
  }

  template <class SourceContainerTypeT, class PackerTypeT>
  auto GetAdditionalBlocks(const SourceContainerTypeT& additionalDataBlocks, PackerTypeT packetType)
  {
    SourceContainerTypeT preallocatedBlocks;

    std::copy_if(
      additionalDataBlocks.cbegin()
      , additionalDataBlocks.cend()
      , std::back_inserter(preallocatedBlocks)
      , [&packetType](const auto& block)->auto
    {
      return block.ownerType == packetType;
    });

    return preallocatedBlocks;
  }

  template <class SourceContainerTypeT, class PackerTypeT>
  auto GetSingleAdditionalBlock(const SourceContainerTypeT& additionalDataBlocks, PackerTypeT packetType)
  {
    auto preallocatedBlocks = GetAdditionalBlocks(additionalDataBlocks, packetType);

    assert(preallocatedBlocks.size() == 1);
    return preallocatedBlocks.front();
  }
} // namespace utils