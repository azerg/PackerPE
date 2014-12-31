#pragma once

#include <Windows.h>
#include <cstdint>
#include <stdexcept>

namespace pelib
{
  class ImageSectionHeader
  {
  public:
    ImageSectionHeader(const char* data, size_t maxData)
    {
      if (maxData < sizeof(IMAGE_SECTION_HEADER))
      {
        throw std::runtime_error("not enough data for section");
      }

      section_data_ = *reinterpret_cast<PIMAGE_SECTION_HEADER>(const_cast<char*>(data));
    }

    std::string Name() const
    {
      return std::string(section_data_.Name, &section_data_.Name[IMAGE_SIZEOF_SHORT_NAME]);
    }

    void SetName(const char* newName, size_t length)
    {
      if (length > IMAGE_SIZEOF_SHORT_NAME)
      {
        throw std::runtime_error("cant update section name: length is too long");
      }
    }

    DWORD VirtualAddress() const { return section_data_.VirtualAddress; }
    void SetVirtualAddress(DWORD virtualAddress){ section_data_.VirtualAddress = virtualAddress; };

    DWORD SizeOfRawData() const { return section_data_.SizeOfRawData; }
    void SetSizeOfRawData(DWORD sizeOfRawData){ section_data_.SizeOfRawData = sizeOfRawData; }

    DWORD PointerToRawData() const { return section_data_.PointerToRawData; }
    void SetPointerToRawData(DWORD pointerToRawData){ section_data_.PointerToRawData = pointerToRawData; }

    char* data(){ return reinterpret_cast<char*>(&section_data_); }
    size_t size() const { return sizeof(IMAGE_SECTION_HEADER); }

  private:
    IMAGE_SECTION_HEADER section_data_;
  };
} // namespace pelib