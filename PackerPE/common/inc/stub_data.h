#pragma once

namespace stub
{
  // todo(azerg) redesign for x64 & x86 etc. required
  typedef struct STUB_DATA_
  {
    DWORD pLoadLibrary;
    DWORD pGetProcAddress;
    DWORD unused_iatnull_0;
    //fpRtlDecompressBuffer pRtlDecompressBuffer; <-- uint64_t?
    DWORD unused_iatnull_1;
    DWORD dwImageBase;
    DWORD dwOriginalEP;
    DWORD dwOriIAT;
    DWORD dwOriginalIVA;        //Original Virtual Address Of Import
    DWORD dwOriginalIS;          //Original Size Of Import
  }STUB_DATA, *PSTUB_DATA;
} // namespace stub