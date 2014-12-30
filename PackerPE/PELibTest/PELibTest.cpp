// PELibTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PELibTest.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include <fstream>

#include "executable.h"

using namespace pelib;

//-------------------------------------------------------------------
std::vector<uint8_t> g_sampleFile;
//-------------------------------------------------------------------

TEST(PElibTest, entry_point_test)
{
  Executable <PE32> testExe(g_sampleFile);

  auto addressOfEntryPoint = testExe.optionalHeader_->AddressOfEntryPoint;
  EXPECT_EQ(addressOfEntryPoint, 0x6e757220);
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
  if (argc != 1)
    return -1;

  g_sampleFile = readFile(argv[0]);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
