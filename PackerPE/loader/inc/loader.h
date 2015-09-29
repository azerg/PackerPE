#pragma once

#include "Windows.h"
#include "stub_data.h"

typedef LONG NTSTATUS, *PNTSTATUS;
typedef NTSTATUS ( __stdcall *fpRtlDecompressBuffer )( USHORT, PUCHAR, ULONG, PUCHAR, ULONG, PULONG );
typedef HMODULE(__stdcall *fpLoadLibraryA)(char*);
typedef FARPROC(__stdcall *fpGetProcAddress)(HMODULE, char*);


int StubEP();
void StartOriginalPE( stub::PSTUB_DATA pStubData );
void UnpackSections(stub::PSTUB_DATA pStubData );
void PrepareOriginalImage(stub::PSTUB_DATA pStubData);
void FixRelocations(IMAGE_BASE_RELOCATION *base_reloc, DWORD relocation_size, DWORD new_imgbase, DWORD old_imgbase);
bool FixImports(stub::PSTUB_DATA pStub, DWORD newBase, IMAGE_IMPORT_DESCRIPTOR *impDesc);

typedef struct _PEB {


    BOOLEAN                 InheritedAddressSpace;
    BOOLEAN                 ReadImageFileExecOptions;
    BOOLEAN                 BeingDebugged;
    BOOLEAN                 Spare;
    HANDLE                  Mutant;
    ULONG                   ImageBaseAddress;
    PVOID                   param1;
    PVOID                   param2;

    PVOID                   SubSystemData;
    PVOID                   ProcessHeap;
    PVOID                   FastPebLock;
    PVOID                   param3;
    PVOID                   param4;

    ULONG                   EnvironmentUpdateCount;
    PVOID                  *KernelCallbackTable;
    PVOID                   EventLogSection;
    PVOID                   EventLog;
    PVOID                   FreeList;

    ULONG                   TlsExpansionCounter;
    PVOID                   TlsBitmap;
    ULONG                   TlsBitmapBits[0x2];
    PVOID                   ReadOnlySharedMemoryBase;
    PVOID                   ReadOnlySharedMemoryHeap;
    PVOID                  *ReadOnlyStaticServerData;
    PVOID                   AnsiCodePageData;
    PVOID                   OemCodePageData;
    PVOID                   UnicodeCaseTableData;
    ULONG                   NumberOfProcessors;
    ULONG                   NtGlobalFlag;
    BYTE                    Spare2[0x4];
    LARGE_INTEGER           CriticalSectionTimeout;
    ULONG                   HeapSegmentReserve;
    ULONG                   HeapSegmentCommit;
    ULONG                   HeapDeCommitTotalFreeThreshold;
    ULONG                   HeapDeCommitFreeBlockThreshold;
    ULONG                   NumberOfHeaps;
    ULONG                   MaximumNumberOfHeaps;
    PVOID                  *ProcessHeaps;

    PVOID                   GdiSharedHandleTable;
    PVOID                   ProcessStarterHelper;
    PVOID                   GdiDCAttributeList;
    PVOID                   LoaderLock;
    ULONG                   OSMajorVersion;
    ULONG                   OSMinorVersion;
    ULONG                   OSBuildNumber;
    ULONG                   OSPlatformId;
    ULONG                   ImageSubSystem;
    ULONG                   ImageSubSystemMajorVersion;
    ULONG                   ImageSubSystemMinorVersion;
    ULONG                   GdiHandleBuffer[0x22];
    ULONG                   PostProcessInitRoutine;
    ULONG                   TlsExpansionBitmap;
    BYTE                    TlsExpansionBitmapBits[0x80];
    ULONG                   SessionId;

} PEB, *PPEB;