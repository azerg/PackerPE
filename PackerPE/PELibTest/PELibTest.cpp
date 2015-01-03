// PELibTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PELibTest.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/filesystem/operations.hpp>

#include "executable.h"
#include "sections_helper.h"

using namespace pelib;

//-------------------------------------------------------------------
std::vector<uint8_t> g_sampleFile;
std::vector<uint8_t> g_sampleFile64;
//-------------------------------------------------------------------

template <class baseType>
class Hex
{
public:
  explicit Hex(baseType n) :
    number_(n){}
  baseType value() const { return number_; }
  bool operator == (const Hex& right) const
  {
    return right.number_ == this->number_;
  }
  bool operator == (const baseType& right) const
  {
    return right == this->number_;
  }
private:
  baseType number_;
};

template <class T>
std::ostream& operator<<(std::ostream& out, const Hex<T>& f)
{
  return out << std::hex << f.value();
}

typedef Hex<uint32_t> Hex32;
typedef Hex<uint64_t> Hex64;

TEST(PElibTest, entry_point_test)
{
  {
    Executable<PE32> testExe(g_sampleFile);

    auto addressOfEntryPoint = testExe.optionalHeader_->AddressOfEntryPoint;
    EXPECT_EQ(Hex32(addressOfEntryPoint), Hex32(0x000014e0));
  }

  {
    Executable<PE64> testExe64(g_sampleFile64);
    auto addressOfEntryPoint = testExe64.optionalHeader_->AddressOfEntryPoint;
    EXPECT_EQ(Hex64(addressOfEntryPoint), Hex64(0x00006094));
  }
}

TEST(PElibTest, sections_test)
{
  Executable<PE32> testExe(g_sampleFile);

  auto sections = testExe.sections_;
  EXPECT_EQ(sections.size(), 8);

  auto testSection = *sections.begin();
  EXPECT_STREQ(testSection.Name().c_str(), ".text");
  EXPECT_TRUE(testSection.IsFlagSet(IMAGE_SCN_CNT_CODE));
  EXPECT_EQ(testSection.IsFlagSet(IMAGE_SCN_CNT_CODE), testSection.IsExecutable());

  int section_counter = 0;
  std::for_each(sections.begin(), sections.end(), [&](ImageSectionHeader& section)
  {
    switch (section_counter)
    {
    case 1: // .text was checked already
      EXPECT_STREQ(section.Name().c_str(), ".data");
      EXPECT_FALSE(section.IsFlagSet(IMAGE_SCN_CNT_CODE));
      break;
    case 2:
      EXPECT_STREQ(section.Name().c_str(), ".rdata");
      break;
    case 3:
      EXPECT_STREQ(section.Name().c_str(), ".bss");
      break;
    case 7:
      EXPECT_STREQ(section.Name().c_str(), ".rsrc");
      break;
    default:
      break;
    }
    section_counter++;
  });

  EXPECT_EQ(section_counter, 8);
}


std::vector<uint8_t> readFile(const char* filename)
{
  std::streampos fileSize;
  std::ifstream file(filename, std::ios::binary);

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> fileData(static_cast<unsigned int>(fileSize));
  file.read((char*)&fileData[0], fileSize);
  return fileData;
}

// Sample files are hardcoded to be located at 
// $(SolutionDir)/SAMPLE_FILES/$(Configuration)
int _tmain(int argc, char* argv[])
{
  auto path = boost::filesystem::current_path().parent_path();
  path /= "SAMPLE_FILES";

  auto x86FilePath = path;
  x86FilePath /= "x86/fzputtygen.exe";
  auto x64FilePath = path;
  x64FilePath /= "x64/notepad.exe";

  g_sampleFile = readFile(x86FilePath.string().c_str());
  g_sampleFile64 = readFile(x64FilePath.string().c_str());

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
