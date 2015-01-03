#pragma once

#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include "sections_helper.h"
#include <vector>

namespace pelib
{
  enum PE_TYPE
  {
    PE_UNKNOWN,
    PE32,
    PE64 // PE32+
  };

  template <typename ContainerType>
  PE_TYPE GetExecutableType(const ContainerType& buffer)
  {
    return GetExecutableType(reinterpret_cast<const char*>(buffer.data()), buffer.size());
  }

  PE_TYPE GetExecutableType(const char* buffer, size_t size)
  {
    // todo: buffer size checks
    char* ptr(const_cast<char*>(buffer));

    ptr += (reinterpret_cast<PIMAGE_DOS_HEADER>(ptr)->e_lfanew) + sizeof(IMAGE_NT_SIGNATURE) + sizeof(IMAGE_FILE_HEADER);

    PIMAGE_OPTIONAL_HEADER32 head = reinterpret_cast<PIMAGE_OPTIONAL_HEADER32>(ptr);

    switch (head->Magic)
    {
    case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
      return PE32;
    case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
      return PE64;
    };

    return PE_UNKNOWN;
  }

  template <typename ImageNtHeadersType>
  class ExecutableBase
  {
    typedef decltype(ImageNtHeadersType::OptionalHeader) ImageOptionalHeader;
  public:
    explicit ExecutableBase(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(reinterpret_cast<const char*>(&fileBuff.front())){}
    explicit ExecutableBase(const char* buffer) :
      buffer_(buffer)
    {
      char* curOffset(const_cast<char*>(buffer));
      dosHeader_ = reinterpret_cast<PIMAGE_DOS_HEADER>(curOffset);
      curOffset += dosHeader_->e_lfanew;

      ImageNtHeadersType* ntHeaders = reinterpret_cast<ImageNtHeadersType*>(curOffset);
      curOffset += sizeof(ImageNtHeadersType);

      fileHeader_ = &ntHeaders->FileHeader;
      optionalHeader_ = &ntHeaders->OptionalHeader;

      sections_ = Sections(reinterpret_cast<PIMAGE_SECTION_HEADER>(curOffset), fileHeader_);
    }

    PIMAGE_DOS_HEADER dosHeader_;
    PIMAGE_FILE_HEADER fileHeader_;
    ImageOptionalHeader* optionalHeader_;
    Sections sections_;

  private:
    const char* buffer_;
  };

  template <PE_TYPE>
  class Executable;

  template <>
  class Executable <PE32> :
    public ExecutableBase<IMAGE_NT_HEADERS>
  {
  public:
    explicit Executable(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(fileBuff){}
    explicit Executable(char* buffer) :
      ExecutableBase(buffer){}
  };

  template <>
  class Executable <PE64> :
    public ExecutableBase<IMAGE_NT_HEADERS64>
  {
  public:
    explicit Executable(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(fileBuff){}
    explicit Executable(char* buffer) :
      ExecutableBase(buffer){}
  };
} //namespace pelib