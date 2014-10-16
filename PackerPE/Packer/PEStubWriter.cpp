#include "StdAfx.h"
#include "PEStubWriter.h"
#include "StubResourceID.h"
#include <boost/scoped_array.hpp>
#include <vector>

namespace PE
{

	PEStubWriter::PEStubWriter(void)
	{
    if ( !StubExists() )
    {
        throw std::runtime_error( "IDR_STUB resource not found. required." );
    }
	}

	PEStubWriter::~PEStubWriter(void)
	{
    FreeResource( m_hResGlob );
	}

  bool PEStubWriter::StubExists()
  {
    m_hResource = FindResource( GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_STUB), RT_RCDATA );
    if ( m_hResource == NULL )
    {
        return false;
    }

    m_hResGlob = LoadResource( GetModuleHandle(NULL), m_hResource );
    if ( m_hResGlob == NULL )
    {
        return false;
    }

    m_cbResStub = SizeofResource( GetModuleHandle(NULL), m_hResource );
    if ( m_cbResStub == 0 )
    {
        return false;
    }

    m_pResStub = LockResource( m_hResGlob );
    if ( m_pResStub == NULL )
    {
        return false;
    }

    return true;
  }

  ULONG_PTR PEStubWriter::GetNewImportTableSize()
  {
    return 2 * sizeof( IMAGE_IMPORT_DESCRIPTOR ) + 3 * sizeof( IMAGE_IMPORT_BY_NAME ) + 6 * sizeof( IMAGE_THUNK_DATA );
  }

	// retrieves size of Stub & Data
	ULONG_PTR PEStubWriter::GetRequiredSize()
	{
		return GetStubSize() + sizeof( STUB_DATA ) + GetNewImportTableSize();
	}

	ULONG_PTR PEStubWriter::GetStubSize()
	{
		return m_cbResStub;
	}

	void PEStubWriter::InsertStub( PIMAGE_SECTION_HEADER pSectionHead, PE::MemPEFile& TargetPE )
	{
		STUB_DATA StubData;

		char* pCurOffset = reinterpret_cast<char*>( pSectionHead->PointerToRawData + TargetPE.GetMemPtr() );
        char* pStubDataAddress = pCurOffset;

		FillStubData( TargetPE, StubData );

		memcpy( pCurOffset, &StubData, sizeof(STUB_DATA) );
		pCurOffset += sizeof(STUB_DATA);

		memcpy( pCurOffset, reinterpret_cast<char*>( m_pResStub ), GetStubSize() );

		SetNewEntryPoint( pSectionHead->VirtualAddress + sizeof(STUB_DATA), TargetPE );

		pCurOffset += GetStubSize();

		//Create names of dll and two functions
		WORD ordinal = 1;
		const CHAR* dllName = "KERNEL32.dll";
		const CHAR* UncompressDllName = "ntdll.dll";
		const CHAR* funcOneName = "LoadLibraryA";
		const CHAR* funcTwoName = "GetProcAddress";
		const CHAR* UncompressFuncName = "RtlDecompressBuffer";
		char* pAddressOfDllName = pCurOffset + 3 * sizeof( IMAGE_IMPORT_DESCRIPTOR );
		char* pAddressOfUncompressDllName = pAddressOfDllName + strlen( dllName ) + 1;
		char* pAddressOfFuncOne = pAddressOfUncompressDllName + strlen( UncompressDllName ) + 1;  
		char* pAddressOfFuncTwo = pAddressOfFuncOne + strlen( funcOneName ) + 3;					//+2, because include ordinal of second func
		char* pAddressOfUncompressFuncName = pAddressOfFuncTwo + strlen( funcTwoName ) + 3;			//+2, because include ordinal of second func
		char* pAddressOfThunkData = pAddressOfUncompressFuncName + strlen( UncompressFuncName ) + 3;//+2, because include ordinal of second func
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfDllName ), dllName, strlen( dllName ) + 1 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfUncompressDllName ), UncompressDllName, strlen( UncompressDllName ) + 1 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfFuncOne ), &ordinal, 2 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfFuncOne + 2 ), funcOneName, strlen( funcOneName ) + 1 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfFuncTwo ), &ordinal, 2 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfFuncTwo + 2 ), funcTwoName, strlen( funcTwoName ) + 1 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfUncompressFuncName ), &ordinal, 2 );
		memcpy(	reinterpret_cast< PVOID > ( pAddressOfUncompressFuncName + 2 ), UncompressFuncName, strlen( UncompressFuncName ) + 1 );

		// Create IMAGE_THUNK_DATA array
		IMAGE_THUNK_DATA thunkData;

		//Add IMAGE_THUNK_DATA for Original FirstThunk
		thunkData.u1.AddressOfData = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfFuncOne - ( DWORD )TargetPE.GetMemPtr() );
		memcpy( reinterpret_cast< PVOID >  ( pAddressOfThunkData ),   &thunkData,    sizeof( IMAGE_THUNK_DATA ));

		thunkData.u1.AddressOfData = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfFuncTwo - ( DWORD )TargetPE.GetMemPtr() );
		memcpy( reinterpret_cast< PVOID > ( pAddressOfThunkData + sizeof( IMAGE_THUNK_DATA ) ), &thunkData,sizeof( IMAGE_THUNK_DATA ) );

		thunkData.u1.AddressOfData = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfUncompressFuncName - ( DWORD )TargetPE.GetMemPtr() );
		char* pAddressOfThunkDataUncompressDll = pAddressOfThunkData + 3 * sizeof( IMAGE_THUNK_DATA );
		memcpy( reinterpret_cast< PVOID > ( pAddressOfThunkDataUncompressDll ), &thunkData, sizeof(IMAGE_THUNK_DATA));

		ZeroMemory( &thunkData, sizeof( IMAGE_THUNK_DATA ) );
		memcpy( reinterpret_cast< PVOID > ( pAddressOfThunkData + 2 * sizeof( IMAGE_THUNK_DATA ) ), &thunkData,sizeof( IMAGE_THUNK_DATA ) );
		memcpy( reinterpret_cast< PVOID > ( pAddressOfThunkDataUncompressDll + sizeof( IMAGE_THUNK_DATA ) ), &thunkData,sizeof( IMAGE_THUNK_DATA ) );

		// Create IMAGE_IMPORT_DESCRIPTOR structure for first dll
		IMAGE_IMPORT_DESCRIPTOR ImageImportDescriptor;
		ImageImportDescriptor.Characteristics = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfThunkData - (DWORD)TargetPE.GetMemPtr() );
		ImageImportDescriptor.TimeDateStamp = 0;
		ImageImportDescriptor.ForwarderChain = 0;
		ImageImportDescriptor.Name = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfDllName - ( DWORD )TargetPE.GetMemPtr() );
		ImageImportDescriptor.FirstThunk = TargetPE.FileOffsetToRva( ( DWORD )pStubDataAddress - ( DWORD )TargetPE.GetMemPtr() );
		// Add new descriptor with dll
		memcpy( reinterpret_cast< PVOID > ( pCurOffset ), &ImageImportDescriptor, sizeof( IMAGE_IMPORT_DESCRIPTOR ) );

		// Create IMAGE_IMPORT_DESCRIPTOR structure for Uncompress dll
		ImageImportDescriptor.Characteristics = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfThunkDataUncompressDll - (DWORD)TargetPE.GetMemPtr() );
		ImageImportDescriptor.TimeDateStamp = 0;
		ImageImportDescriptor.ForwarderChain = 0;
		ImageImportDescriptor.Name = TargetPE.FileOffsetToRva( ( DWORD )pAddressOfUncompressDllName - ( DWORD )TargetPE.GetMemPtr() );
		ImageImportDescriptor.FirstThunk = TargetPE.FileOffsetToRva( ( DWORD ) pStubDataAddress + sizeof(DWORD)*3 - ( DWORD )TargetPE.GetMemPtr()/*( DWORD )pAddressOfFirstThunkUncompressDll - ( DWORD )TargetPE.GetMemPtr()*/ );
		// Add new descriptor with Uncompress dll
		memcpy( reinterpret_cast< PVOID > ( pCurOffset + sizeof( IMAGE_IMPORT_DESCRIPTOR ) ), &ImageImportDescriptor, sizeof( IMAGE_IMPORT_DESCRIPTOR ) );

		// Fix address in DataDirectory
		TargetPE.GetOptionalHead32()->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = TargetPE.FileOffsetToRva( ( DWORD )pCurOffset - ( DWORD )TargetPE.GetMemPtr() );
		TargetPE.GetOptionalHead32()->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = sizeof( IMAGE_IMPORT_DESCRIPTOR );

		// Create final zero-fields IMAGE_IMPORT_DESCRIPTOR structure
		IMAGE_IMPORT_DESCRIPTOR finalRecord;

		ZeroMemory( &finalRecord, sizeof( IMAGE_IMPORT_DESCRIPTOR ) );
		memcpy( reinterpret_cast< PVOID > ( pCurOffset + 2 * sizeof( IMAGE_IMPORT_DESCRIPTOR ) ), &finalRecord, sizeof( IMAGE_IMPORT_DESCRIPTOR ) );

	}

    void PEStubWriter::FillStubData( PE::MemPEFile& TargetPE, __out STUB_DATA& StubData )
    {
        // fill EntryPoint addr
        StubData.dwOriginalEP = TargetPE.GetOptionalHead32()->AddressOfEntryPoint;
		StubData.dwOriginalIVA = TargetPE.GetOptionalHead32()->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		StubData.dwOriginalIS = TargetPE.GetOptionalHead32()->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
    }

    void PEStubWriter::SetNewEntryPoint( ULONG dwNewEntryPoint, PE::MemPEFile& TargetPE )
    {
        TargetPE.GetOptionalHead32()->AddressOfEntryPoint = dwNewEntryPoint;
    }

	BOOL PEStubWriter::CompressSections( PE::MemPEFile& SourcePE, PE::MemPEFile& TargetPE )
	{
		const DWORD dwBufferSize = 0x8000000;
		HMODULE ntdll = GetModuleHandle(L"ntdll.dll");

		fpRtlDecompressBuffer RtlDecompressBuffer = ( fpRtlDecompressBuffer ) GetProcAddress( ntdll, "RtlDecompressBuffer" );
		fpRtlCompressBuffer RtlCompressBuffer = ( fpRtlCompressBuffer ) GetProcAddress( ntdll, "RtlCompressBuffer" );
		fpRtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize = ( fpRtlGetCompressionWorkSpaceSize ) GetProcAddress( ntdll, "RtlGetCompressionWorkSpaceSize" );

		//Get compression workspace
		ULONG ws_size, fs_size;
		NTSTATUS status;
		status = RtlGetCompressionWorkSpaceSize( COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_MAXIMUM, &ws_size, &fs_size );
		if( status >= 0x8000000 )
		{
			MessageBox( 0, L"RtlGetCompressionWorkSpaceSize returned error", L"Error", 0);
			return FALSE;
		}

    //create workspace
    std::vector<char> WorkSpace(ws_size);

    //setting get section's parameters before compression
    DWORD dwNumberOfSection = SourcePE.GetFileHead()->NumberOfSections;
    PIMAGE_SECTION_HEADER pCurrentSectionHead = SourcePE.GetSectionHead();
    PIMAGE_SECTION_HEADER pNewSectionHead = TargetPE.GetSectionHead();

    //create buffers for compressed data
    std::vector<char> CompressedBuffer;
    ULONG PackedSize;
    ULONG AlignPackedSize;
    ULONG diffSize;
    DWORD cbLastSection = 0;
    DWORD dwFileAlignment = TargetPE.GetOptionalHead32()->FileAlignment;
    DWORD newFileOffset = 0;

    for( int i = 0; i < dwNumberOfSection; ++i, ++pCurrentSectionHead, ++pNewSectionHead )
    {
        if ( pCurrentSectionHead->SizeOfRawData == 0 )
        {
            continue;
        }

        // don't process resources compression. Just copy compete one into packed file
        if ( strcmp( reinterpret_cast<char*>( &pCurrentSectionHead->Name[0] ), ".rsrc" ) == 0 )
        {
            PUCHAR pSourceBuffer = reinterpret_cast<PUCHAR>( pCurrentSectionHead->PointerToRawData + SourcePE.GetMemPtr() );

            memcpy( reinterpret_cast<void*>( newFileOffset ), pSourceBuffer, pCurrentSectionHead->SizeOfRawData );

            pNewSectionHead->PointerToRawData = cbLastSection;
            pNewSectionHead->SizeOfRawData = pNewSectionHead->SizeOfRawData;
            cbLastSection += pNewSectionHead->SizeOfRawData;
            newFileOffset += pNewSectionHead->SizeOfRawData;
            continue;
        }

        // first valid section found. Save its physical address as basis for new insertions
        if ( newFileOffset == 0 )
        {
            newFileOffset = pNewSectionHead->PointerToRawData + TargetPE.GetMemPtr();
            cbLastSection = pNewSectionHead->PointerToRawData;
        }

        // todo(azerg): remove hardcoded RWX section value
        pNewSectionHead->Characteristics |= IMAGE_SCN_MEM_WRITE;

        PUCHAR pSourceBuffer = reinterpret_cast<PUCHAR>( pCurrentSectionHead->PointerToRawData + SourcePE.GetMemPtr() );
        PackedSize = 0;

        status = RtlCompressBuffer(
            COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_MAXIMUM,
            pSourceBuffer,
            pCurrentSectionHead->SizeOfRawData,
            reinterpret_cast<PUCHAR>( newFileOffset ),		//out
            pCurrentSectionHead->SizeOfRawData,
            4096,
            &PackedSize,			//out
            &WorkSpace.front()
            );

        AlignPackedSize = TargetPE.AlignUp( PackedSize, dwFileAlignment );
        diffSize = AlignPackedSize - PackedSize;
        // Append section buffer with zeroes to fit FileAlignment value
        if( diffSize > 0 )
        {
            ZeroMemory( reinterpret_cast<void*>( newFileOffset + PackedSize ), sizeof( diffSize ) );
        }

        // setup new section header values, regards to new sizings
        pNewSectionHead->PointerToRawData = cbLastSection;
        pNewSectionHead->SizeOfRawData = AlignPackedSize;
        cbLastSection += AlignPackedSize;
        newFileOffset += AlignPackedSize;
    }

    // Update SizeOfImage
    TargetPE.GetOptionalHead32()->SizeOfImage = newFileOffset - TargetPE.GetMemPtr(); 

    TargetPE.CutFile( TargetPE.GetOptionalHead32()->SizeOfImage );

		return TRUE;
	}

}
