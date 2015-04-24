#include "file_utils.h"

#include <fstream>

namespace file_utils
{
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

  void writeFile(const char* filename, uint64_t offset, const char* pData, uint32_t cbData)
  {
    std::fstream file(filename, std::fstream::out | std::fstream::binary | std::fstream::in | std::fstream::app);
    file.seekp(offset, std::ios::beg);
    file.write(pData, cbData);
    file.close();
  }
} // namespace file_utils