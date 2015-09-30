#pragma once
#include "Windows.h"

namespace stub
{
  typedef DWORD pointer_t;
  // todo(azerg) redesign for x64 & x86 etc. required
  typedef struct STUB_DATA_
  {
    pointer_t pLoadLibrary; // <-- IAT should point here in Alpha
    pointer_t pGetProcAddress;
    pointer_t pRtlDecompressBuffer;
    DWORD unused_iatnull_1; // <-- is filled by OS with zeroes, while filling iat data
    DWORD dwNewImageBase;
    DWORD dwOriginalImageBase;
    DWORD dwOriginalEP;
    // todo(azerg): maybe replace code below with complete PE-header :D
    DWORD dwOriginalRelocVA;
    DWORD dwOriginalRelocSize;
    DWORD dwOriginalImportVA;
    DWORD dwOriginalImportSize;
  }STUB_DATA, *PSTUB_DATA;

  #define STUB_IAT_SIZE 16
} // namespace stub