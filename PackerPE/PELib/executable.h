#pragma once

#include <Windows.h>
#include <cstdint>
#include <stdexcept>
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
  PE_TYPE GetExecutableType(ContainerType buffer)
  {
    return GetExecutableType(reinterpret_cast<const uintptr_t>(buffer.data()), buffer.size());
  }

  PE_TYPE GetExecutableType(const uintptr_t buffer, size_t size)
  {
    // todo: buffer size checks
    auto ptr(buffer);

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

  template <typename PImageOptionalHeader>
  class ExecutableBase
  {
  public:
    explicit ExecutableBase(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(reinterpret_cast<uintptr_t>(&fileBuff.front())){}
    explicit ExecutableBase(const uintptr_t buffer) :
      buffer_(buffer)
    {
      uintptr_t curOffset(buffer);
      dosHeader_ = reinterpret_cast<PIMAGE_DOS_HEADER>(buffer);
      curOffset += sizeof(IMAGE_DOS_HEADER);

      fileHeader_ = reinterpret_cast<PIMAGE_FILE_HEADER>(curOffset);
      curOffset += sizeof(IMAGE_FILE_HEADER);

      optionalHeader_ = reinterpret_cast<PImageOptionalHeader>(curOffset);
      curOffset += sizeof(PImageOptionalHeader);
    }

    PIMAGE_DOS_HEADER dosHeader_;
    PIMAGE_FILE_HEADER fileHeader_;
    PImageOptionalHeader optionalHeader_;

  private:
    const uintptr_t buffer_;
  };

  template <PE_TYPE>
  class Executable;

  template <>
  class Executable <PE32> :
    public ExecutableBase<PIMAGE_OPTIONAL_HEADER32>
  {
  public:
    explicit Executable(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(fileBuff){}
    explicit Executable(uintptr_t buffer) :
      ExecutableBase(buffer){}
  };

  template <>
  class Executable <PE64> :
    public ExecutableBase<PIMAGE_OPTIONAL_HEADER64>
  {
  public:
    explicit Executable(const std::vector<uint8_t>& fileBuff) :
      ExecutableBase(fileBuff){}
    explicit Executable(uintptr_t buffer) :
      ExecutableBase(buffer){}
  };
} //namespace pelib