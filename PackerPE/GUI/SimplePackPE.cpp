#include "StdAfx.h"
#include "SimplePackPE.h"
#include "../PELib/MemFile.h"
#include "../Packer/PEStubWriter.h"
#include <memory>
#include <vector>
#include <conio.h>
//#include "../LzmaLib/Types.h"
//#include "../LzmaLib/LzmaLib.h"

#define LZMA_DEF_RATIO 0;

typedef std::vector<unsigned char> UcharVt;

CSimplePackPE::CSimplePackPE( void )
{
}

CSimplePackPE::~CSimplePackPE( void )
{
}

bool CSimplePackPE::IsValidPE( CString szPathName )
{
  try
  {
        PE::MemPEFile MemPE( szPathName, PE::MEMFILE_ACCESS::MA_READ );

    if ( !MemPE.IsValidPEFile() )
    {
      MessageBox( 0,L"Not valid PE-file",L"Error",0 );
      return false;
    }
  }
  catch ( const std::runtime_error& )
  {
    MessageBox(0,L"Error opening selected file",L"Error",0);
    return false;
  }
  catch ( const std::logic_error& )
  {
    MessageBox(0,L"Dev error occured. Check.",L"Error",0);
    return false;
  }
  return true;
}

BOOL CSimplePackPE::PackPE( CString szPathName, CString szNewPathName )
{
  CopyFile( szPathName, szNewPathName, FALSE );

    PE::PEStubWriter StubWriter;

    PE::MemPEFile targetPEFile( szNewPathName, PE::MEMFILE_ACCESS::MA_ALL );
    PE::MemPEFile sourcePEFile( szPathName, PE::MEMFILE_ACCESS::MA_READ );

    if ( !targetPEFile.IsValidPEFile() )
    {
        MessageBox( 0,L"Not valid PE-file",L"Error",0 );
        return FALSE;
    }

  if( StubWriter.CompressSections( sourcePEFile, targetPEFile ) != TRUE)
  {
    MessageBox( 0,L"Compression failure",L"Error",0 );
    return FALSE;
  }

  PIMAGE_SECTION_HEADER pNewHead = targetPEFile.AddPESection( ".sec0", StubWriter.GetRequiredSize(),
        IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ );

  StubWriter.InsertStub( pNewHead, targetPEFile );

  return TRUE;
}
