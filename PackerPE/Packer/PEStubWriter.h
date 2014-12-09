#pragma once

#include <Windows.h>
#include "../PELib/MemFile.h"
#include "Stub.h"

#define COMPRESSION_FORMAT_LZNT1     (0x0002)
#define COMPRESSION_ENGINE_MAXIMUM   (0x0100)
#define COMPRESS_SAFE 1024

namespace PE
{
  typedef LONG NTSTATUS, *PNTSTATUS;
  typedef NTSTATUS ( __stdcall *fpRtlDecompressBuffer )( USHORT, PUCHAR, ULONG, PUCHAR, ULONG, PULONG );
  typedef NTSTATUS ( __stdcall *fpRtlCompressBuffer )( USHORT, PUCHAR, ULONG, PUCHAR, ULONG, ULONG, PULONG, PVOID );
  typedef NTSTATUS ( __stdcall *fpRtlGetCompressionWorkSpaceSize )( USHORT, PULONG, PULONG );

  class PEStubWriter
  {
  public:
    PEStubWriter(void);
    ~PEStubWriter(void);
    void InsertStub( PIMAGE_SECTION_HEADER pSectionHead, PE::MemPEFile& TargetPE );
    ULONG_PTR GetRequiredSize();
    BOOL CompressSections( PE::MemPEFile& SourcePE, PE::MemPEFile& TargetPE );

  private:
    ULONG_PTR GetStubSize();
    ULONG_PTR GetNewImportTableSize();
    bool StubExists();
    void FillStubData( PE::MemPEFile& TargetPE, __out STUB_DATA& StubData );
    void SetNewEntryPoint( ULONG dwNewEntryPoint, PE::MemPEFile& TargetPE );

    HRSRC m_hResource;
    HGLOBAL m_hResGlob;
    LPVOID m_pResStub;
    DWORD m_cbResStub;

  };

}
