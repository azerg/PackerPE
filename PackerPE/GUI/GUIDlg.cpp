// GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGUIDlg dialog




CGUIDlg::CGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPATH, m_EditPath);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDPACK, &CGUIDlg::OnBnClickedPack)
	ON_BN_CLICKED(IDC_OPEN, &CGUIDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDCLOSE, &CGUIDlg::OnBnClickedClose)
//	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CGUIDlg message handlers

BOOL CGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGUIDlg::OnBnClickedOpen()
{
	// szFilters is a text string that includes two file name filters:
	wchar_t szFilters[] = L"Exe Files(*.exe)|*.exe|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".exe".
	CFileDialog fileDlg( TRUE, L"exe", L"",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this );

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal() == IDOK )
	{
		if( this->m_SimplePackPE.IsValidPE( fileDlg.GetPathName() ) )
		{
			this->m_EditPath.SetWindowText( fileDlg.GetPathName() );
		}
	}
}

void CGUIDlg::OnBnClickedPack()
{
	CString szEditPath;
	this->m_EditPath.GetWindowText( szEditPath );
	if( szEditPath != L"")
	{
		this->m_ProgressCtrl.ShowWindow( SW_SHOW );
		this->m_ProgressCtrl.SetPos( 0 );
		if( this->m_SimplePackPE.IsValidPE( szEditPath ) )
		{
			this->m_ProgressCtrl.SetPos( 25 );
			// szFilters is a text string that includes two file name filters:
			wchar_t szFilters[] = L"Exe Files(*.exe)|*.exe|All Files (*.*)|*.*||";
			// Create a Save dialog; the default file name extension is ".exe".
			CFileDialog fileDlg (FALSE, L"exe", L"",OFN_FILEMUSTEXIST, szFilters, this);
			// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
			// returns IDOK.
			if( fileDlg.DoModal() == IDOK )
			{
				if( this->m_SimplePackPE.PackPE( szEditPath, fileDlg.GetPathName() ) == TRUE)
				{
					this->m_ProgressCtrl.SetPos( 100 );
					MessageBox(L"Pack done ...",L"Message",0);
				}
				else
				{
					this->m_ProgressCtrl.SetPos( 0 );
				}
			}
		}
	}
	else
	{
		MessageBox(L"Do not enter the path",L"Error",0);
	}
}

void CGUIDlg::OnBnClickedClose()
{
	this->EndDialog(NULL);	
}
