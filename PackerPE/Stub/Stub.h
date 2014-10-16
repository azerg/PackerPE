#pragma once

#include "resource.h"

typedef LONG NTSTATUS, *PNTSTATUS;
typedef NTSTATUS ( __stdcall *fpRtlDecompressBuffer )( USHORT, PUCHAR, ULONG, PUCHAR, ULONG, PULONG );

typedef struct STUB_DATA_
{
    DWORD pLoadLibrary;
    DWORD pGetProcAddress;
    DWORD unused_iatnull_0;
    fpRtlDecompressBuffer pRtlDecompressBuffer;
    DWORD unused_iatnull_1;
    DWORD dwImageBase;
	DWORD dwOriginalEP;
	DWORD dwOriIAT;
	DWORD dwOriginalIVA;				//Original Virtual Address Of Import
	DWORD dwOriginalIS;					//Original Size Of Import
}STUB_DATA, *PSTUB_DATA;


int StubEP();
void StartOriginalPE( PSTUB_DATA pStubData );
void UnpackSections( PSTUB_DATA pStubData );



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