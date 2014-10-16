// EditDrop.cpp : implementation file
//

#include "stdafx.h"
#include "GUI.h"
#include "EditDrop.h"
#include "SimplePackPE.h"


// CEditDrop

IMPLEMENT_DYNAMIC( CEditDrop, CEdit )

CEditDrop::CEditDrop()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	EnableAutomation();
}

CEditDrop::~CEditDrop()
{
}

void CEditDrop::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CEdit::OnFinalRelease();
}


BEGIN_MESSAGE_MAP( CEditDrop, CEdit )
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP( CEditDrop, CEdit )
END_DISPATCH_MAP()

// Note: we add support for IID_IEditDrop to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {975AB54D-C3F5-4BBE-8B81-F36DA2C3E0CF}
static const IID IID_IEditDrop =
{ 0x975AB54D, 0xC3F5, 0x4BBE, { 0x8B, 0x81, 0xF3, 0x6D, 0xA2, 0xC3, 0xE0, 0xCF } };

BEGIN_INTERFACE_MAP( CEditDrop, CEdit )
	INTERFACE_PART( CEditDrop, IID_IEditDrop, Dispatch )
END_INTERFACE_MAP()


// CEditDrop message handlers

afx_msg void CEditDrop::OnDropFiles( HDROP hDropInfo ) 
{

	UINT  uNumFiles;
	wchar_t szFileName[512];
	// Get the # of files being dropped.
	uNumFiles = DragQueryFile ( hDropInfo, -1, NULL, 0 );
	if ( uNumFiles > 1 )
	{
		MessageBox(L"Added only the first file ...",L"Message",0);
	}

	DragQueryFile( hDropInfo, 0, szFileName, 512 );

	CSimplePackPE simplePackPE;
	if( simplePackPE.IsValidPE( szFileName ) )
	{
		this->SetWindowText( szFileName );
	}
	
	// Free up memory.
	DragFinish ( hDropInfo );

}