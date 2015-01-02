#pragma once

#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#ifndef min
  #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace pelib
{
#define  SectionGetterAndSetter(memberName) \
  DWORD memberName() const { return section_data_.##memberName; } \
  void Set##memberName(DWORD val){ section_data_.##memberName = val; }

  class ImageSectionHeader
  {
  public:
    ImageSectionHeader(ImageSectionHeader& right):
      section_data_(right.section_data_){}
    ImageSectionHeader(std::string name):
      section_data_{}
    {
      if (name.length() > IMAGE_SIZEOF_SHORT_NAME)
      {
        throw std::runtime_error("Error constructing sectionHeader obj -> name is too long");
      }
      SetName(name.c_str(), name.length());
    }
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
      auto nameLen = min(IMAGE_SIZEOF_SHORT_NAME, std::strlen(reinterpret_cast<char*>(const_cast<BYTE*>(&section_data_.Name[0]))));
      return std::string(section_data_.Name, &section_data_.Name[nameLen]);
    }

    void SetName(const char* newName, size_t length)
    {
      if (length > IMAGE_SIZEOF_SHORT_NAME)
      {
        throw std::runtime_error("cant update section name: length is too long");
      }
    }

    SectionGetterAndSetter(VirtualAddress);
    SectionGetterAndSetter(SizeOfRawData);
    SectionGetterAndSetter(PointerToRawData);

    char* data(){ return reinterpret_cast<char*>(&section_data_); }
    size_t size() const { return sizeof(IMAGE_SECTION_HEADER); }
  private:
    IMAGE_SECTION_HEADER section_data_;
  };

  class SectionsIterator:
    public std::iterator<std::forward_iterator_tag, ImageSectionHeader>
  {
  public:
    SectionsIterator(SectionsIterator& right):
      section_(right.section_)
      , buffer_(right.buffer_){}
    SectionsIterator(PIMAGE_SECTION_HEADER buffer) :
      buffer_(buffer)
      , section_(reinterpret_cast<const char*>(buffer), sizeof(IMAGE_SECTION_HEADER)){}

    SectionsIterator& operator++()
    {
      ++buffer_;
      section_ = ImageSectionHeader(reinterpret_cast<const char*>(buffer_), sizeof(IMAGE_SECTION_HEADER));
      return *this;
    }

    bool operator==(const SectionsIterator& right) { return buffer_ == right.buffer_; }
    bool operator!=(const SectionsIterator& right) { return !operator==(right); }
    ImageSectionHeader& operator*() { return section_; }

  private:
    ImageSectionHeader section_;
    PIMAGE_SECTION_HEADER buffer_;
  };

  class Sections
  {
  public:
    Sections(){}
    Sections(PIMAGE_SECTION_HEADER buffer, PIMAGE_FILE_HEADER fileHeader) :
      buffer_(buffer)
      , numberOfSections_(fileHeader->NumberOfSections)
    {}
    SectionsIterator begin(){ return SectionsIterator(buffer_); };
    SectionsIterator end(){ return SectionsIterator(buffer_ + numberOfSections_); };

    size_t size() const { return numberOfSections_; }

  private:
    size_t numberOfSections_;
    PIMAGE_SECTION_HEADER buffer_;
  };

} // namespace pelib