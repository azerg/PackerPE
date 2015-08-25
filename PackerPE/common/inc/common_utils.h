#pragma once

#include <type_traits>
#include <vector>
#include <string>
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
} // namespace utils