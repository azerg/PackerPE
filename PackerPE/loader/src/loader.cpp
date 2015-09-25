// SimplePackPE.cpp : Defines the entry point for the console application.
//

#include "includes.h"
#include "loader.h"

int StubEP()
{
  stub::PSTUB_DATA pStubData;
  // get address of STUB_DATA
  __asm{
    jmp __data_raw_end
  __data_raw:
    nop                 // <-- packer will write relative offset of stub data here
    nop
    nop
    nop
  __data_raw_end:
    call __getmyaddr
  __getmyaddr:
    pop eax             // <-- current eip in eax
    sub eax, 4 + 5      // <-- get VA of __data_raw:  [4 - count of nops(sizeof stubDataOffset)] + [5-sizeof call __getmyaddr]
    mov ecx, eax
    sub eax, [ecx]      // current IEP - relativeOffset == pStubData address
    mov pStubData, eax
  }

  PPEB Peb;
  __asm {
    mov eax, FS:[0x30];
    mov Peb, eax
    nop
  }

  pStubData->dwOriginalEP += Peb->ImageBaseAddress;
  pStubData->dwNewImageBase = Peb->ImageBaseAddress;

  PrepareOriginalImage(pStubData);
  //UnpackSections(pStubData); todo(azerg): nothing is packed yet
  StartOriginalPE(pStubData);

  return 0;
}

void StartOriginalPE(stub::PSTUB_DATA pStubData)
{
  auto pOriginalEP = reinterpret_cast<LPVOID>(pStubData->dwOriginalEP);
  __asm
  {
    jmp pOriginalEP
  }
}

void UnpackSections(stub::PSTUB_DATA pStubData)
{
  PIMAGE_DOS_HEADER pDOSHead = reinterpret_cast<PIMAGE_DOS_HEADER>(pStubData->dwNewImageBase);
  PIMAGE_NT_HEADERS32 pNtHead = reinterpret_cast<PIMAGE_NT_HEADERS32>(reinterpret_cast<char*>(pDOSHead) + pDOSHead->e_lfanew);
  DWORD dwSizeOfOptHead = pNtHead->FileHeader.SizeOfOptionalHeader;

  PIMAGE_SECTION_HEADER pSectionHead = reinterpret_cast<PIMAGE_SECTION_HEADER>(reinterpret_cast<char*>(&pNtHead->OptionalHeader) + dwSizeOfOptHead);
  DWORD dwFileAlignment = pNtHead->OptionalHeader.FileAlignment;
  DWORD dwNumberOfSections = pNtHead->FileHeader.NumberOfSections;

  DWORD dwUnpackedSize = 0;
  for (DWORD i = 0; i < dwNumberOfSections; ++i, ++pSectionHead)
  {
    if (pSectionHead->SizeOfRawData == 0)
    {
      continue;
    }

    DWORD dwRsrc = 0x72737263; // "rsrc"
    if (memcmp(reinterpret_cast<char*>(&pSectionHead->Name[1]), reinterpret_cast<char*>(&dwRsrc), 4) == 0)
    {
      continue;
    }

    ((fpRtlDecompressBuffer)pStubData->pRtlDecompressBuffer)(
      COMPRESSION_FORMAT_LZNT1,
      reinterpret_cast<PUCHAR>(pStubData->dwNewImageBase + pSectionHead->VirtualAddress),
      pSectionHead->SizeOfRawData,
      reinterpret_cast<PUCHAR>(pStubData->dwNewImageBase + pSectionHead->VirtualAddress),
      pSectionHead->Misc.VirtualSize,
      reinterpret_cast<PULONG>(dwUnpackedSize));
  }
}

void PrepareOriginalImage(stub::PSTUB_DATA pStubData)
{
  FixRelocations((PIMAGE_BASE_RELOCATION)(pStubData->dwNewImageBase + pStubData->dwOriginalRelocVA), pStubData->dwOriginalRelocSize, pStubData->dwNewImageBase, pStubData->dwOriginalImageBase);
}

//----------------------------------------------------------------------------------------------------------------
// Ripped code somewhere from the web

//   Pietrek's macro
//
//   MakePtr is a macro that allows you to easily add to values (including
//   pointers) together without dealing with C's pointer arithmetic.  It
//   essentially treats the last two parameters as DWORDs.  The first
//   parameter is used to typecast the result to the appropriate pointer type.
#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD_PTR)(ptr) + (DWORD_PTR)(addValue))

//   This one is mine, but obviously..."adapted" from matt's original idea =p
#define MakeDelta(cast, x, y) (cast) ( (DWORD_PTR)(x) - (DWORD_PTR)(y))

//   Matt Pietrek's function
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, PIMAGE_NT_HEADERS pNTHeader)
{
  PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
  unsigned int i;

  for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++){
    // This 3 line idiocy is because Watcom's linker actually sets the
    // Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
    DWORD size = section->Misc.VirtualSize;
    if (0 == size)
      size = section->SizeOfRawData;

    // Is the RVA within this section?
    if ((rva >= section->VirtualAddress) &&
      (rva < (section->VirtualAddress + size)))
      return section;
  }

  return 0;
}

//   This function is also Pietrek's
LPVOID GetPtrFromRVA(DWORD rva, IMAGE_NT_HEADERS *pNTHeader, PBYTE imageBase)
{
  PIMAGE_SECTION_HEADER pSectionHdr;
  INT delta;

  pSectionHdr = GetEnclosingSectionHeader(rva, pNTHeader);
  if (!pSectionHdr)
    return 0;

  delta = (INT)(pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData);
  return (PVOID)(imageBase + rva - delta);
}

bool FixImports(stub::PSTUB_DATA pStub, void *base, IMAGE_NT_HEADERS *ntHd, IMAGE_IMPORT_DESCRIPTOR *impDesc)
{
  char* module;

  //   Loop through all the required modules
  while ((module = (char*)GetPtrFromRVA((DWORD)(impDesc->Name), ntHd, (PBYTE)base))){
    //   If the library is already loaded(like kernel32.dll or ntdll.dll) LoadLibrary will
    //   just return the handle to that module.
    HMODULE hMod = ((fpLoadLibraryA)pStub->pLoadLibrary)(module);

    //   Lookup the first import thunk for this module
    //   NOTE: It is possible this module could forward functions...which is something
    //   that I really should handle.  Maybe i'll add support for forwared functions
    //   a little bit later.
    IMAGE_THUNK_DATA *itd =
      (IMAGE_THUNK_DATA *)GetPtrFromRVA((DWORD)(impDesc->FirstThunk), ntHd, (PBYTE)base);

    while (itd->u1.AddressOfData){
      IMAGE_IMPORT_BY_NAME *iibn;
      iibn = (IMAGE_IMPORT_BY_NAME *)GetPtrFromRVA((DWORD)(itd->u1.AddressOfData), ntHd, (PBYTE)base);

      itd->u1.Function = MakePtr(DWORD, ((fpGetProcAddress)pStub->pGetProcAddress)(hMod, (char *)iibn->Name), 0);

      itd++;
    }
    impDesc++;
  }

  return true;
}

/* void FixRelocations
* Function for rebasing a program to a new image base
* IMAGE_BASE_RELOCATION *base_reloc - The relocation section of the image to be used
* DWORD relocation_size - Size of the relocation section
* DWORD new_imgbase - Location in memory of the new image base
* DWORD old_imgbase - Location in memory of the old image base
*/
void FixRelocations(IMAGE_BASE_RELOCATION *base_reloc, DWORD relocation_size, DWORD new_imgbase, DWORD old_imgbase)
{
  IMAGE_BASE_RELOCATION *cur_reloc = base_reloc, *reloc_end;

  //Calculate the difference between the old image base and the new
  DWORD delta = new_imgbase - old_imgbase;

  //Calculate the end of the relocation section
  reloc_end = PIMAGE_BASE_RELOCATION((char *)base_reloc + relocation_size);

  //Loop through the IMAGE_BASE_RELOCATION structures
  while (cur_reloc < reloc_end && cur_reloc->VirtualAddress) {

    //Determine the number of relocations in this IMAGE_BASE_RELOCATION structure
    DWORD count = (cur_reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

    //Grab the current relocation entry. The plus one is to account for the loop below
    WORD *cur_entry = (WORD *)(cur_reloc + 1);

    //Grab the address in new memory where the relocation will occur
    void *page_va = (void *)((char *)new_imgbase + cur_reloc->VirtualAddress);

    //Loop through each of the relocations in the current IMAGE_BASE_RELOCATION structure
    while (count--) {

      /* is valid x86 relocation? */
      if (*cur_entry >> 12 == IMAGE_REL_BASED_HIGHLOW)
        //Add the delta. The 0x0fff ands out the type of relocation
        *(DWORD *)((char *)page_va + (*cur_entry & 0x0fff)) += delta;

      //Move to the next entry
      cur_entry++;

    }

    /* advance to the next one */
    cur_reloc = (IMAGE_BASE_RELOCATION *)((char *)cur_reloc + cur_reloc->SizeOfBlock);

  }
}


