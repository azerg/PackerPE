#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace file_utils
{
  std::vector<uint8_t> readFile(const char* filename);
  std::vector<uint8_t> readFile(const std::string& filename);
  void writeFile(const char* filename, uint32_t offset, const char* pData, uint32_t cbData);
  void appendToFile(const char* filename, const char* pData, uint32_t cbData);
} // namespace file_utils