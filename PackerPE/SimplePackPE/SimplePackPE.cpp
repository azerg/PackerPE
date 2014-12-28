// SimplePackPE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "executable.h"
#include <fstream>
#include <vector>

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
  auto fileBuff = readFile(argv[0]);

  auto peType = GetExecutableType(fileBuff);

  if (peType == PE32)
  {
    Executable<PE32> executable(fileBuff.front());
  }

  return 0;
}

