#pragma once

#include "../PELib/MemFile.h"

class CSimplePackPE
{
public:
  CSimplePackPE( void );
  virtual ~CSimplePackPE( void );
  bool IsValidPE( CString szPathName );
  BOOL PackPE( CString szPathName, CString szNewPathName );
};
