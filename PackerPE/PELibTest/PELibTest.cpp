// PELibTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PELibTest.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include <fstream>
#include <algorithm>

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
    EXPECT_EQ(Hex32(addressOfEntryPoint), Hex32(0x000a79c7));
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

  EXPECT_STREQ((*sections.begin()).Name().c_str(), ".textbss");

  std::for_each(sections.begin(), sections.end(), [&](ImageSectionHeader& section)
  {
    std::cout << section.Name() << std::endl;
  });
}


std::vector<uint8_t> readFile(const _TCHAR* filename)
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

int _tmain(int argc, _TCHAR* argv[])
{
  if (argc != 3)
    return -1;

  g_sampleFile = readFile(argv[1]);
  g_sampleFile64 = readFile(argv[2]);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
