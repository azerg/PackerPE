#pragma once

#include <Windows.h>
#include <WinNT.h>
#include <iostream>

namespace PE
{
  
  class PEFile
  {  
  public:
    PEFile(ULONG_PTR pFileBuff){ Reset(pFileBuff); }
    PEFile(){}
    virtual ~PEFile(void){}
    bool IsValidPEFile();
    void Reset( ULONG_PTR pFileBuff ){ m_pFileBuff = pFileBuff; };

    PIMAGE_DOS_HEADER GetDOSHead();
    PIMAGE_NT_HEADERS32 GetNtHeaders32();
    PIMAGE_FILE_HEADER GetFileHead();
    PIMAGE_OPTIONAL_HEADER32 GetOptionalHead32();
    PIMAGE_SECTION_HEADER GetSectionHead();
    PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor();
    PIMAGE_THUNK_DATA GetThunkData( PIMAGE_IMPORT_DESCRIPTOR pImportDescription );
    PIMAGE_IMPORT_BY_NAME GetImportByName( PIMAGE_THUNK_DATA pThunkData );
    WORD GetNumberOfImportedDlls();  
    DWORD RvaToFileOffset(DWORD rva);
    DWORD FileOffsetToRva(DWORD fileOffset);

  protected:
    PIMAGE_SECTION_HEADER AddSection( const char* szName, ULONG cbSizeOfRawData, ULONG Characteristics );
    
    ULONG_PTR m_pFileBuff;
  };

  static DWORD AlignDown(DWORD val, DWORD align)
  {
    return (val & ~(align - 1));
  }
  static DWORD AlignUp(DWORD val, DWORD align)
  {
    return ((val & (align - 1)) ? AlignDown(val, align) + align : val);
  }
}
