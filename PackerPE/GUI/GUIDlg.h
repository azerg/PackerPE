// GUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SimplePackPE.h"
#include "EditDrop.h"


// CGUIDlg dialog
class CGUIDlg : public CDialog
{
// Construction
public:
  CGUIDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
  enum { IDD = IDD_GUI_DIALOG };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support


// Implementation
protected:
  HICON m_hIcon;
  CSimplePackPE m_SimplePackPE;

  // Generated message map functions
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
  CEditDrop m_EditPath;
  CProgressCtrl m_ProgressCtrl;
public:
  afx_msg void OnBnClickedPack();
  afx_msg void OnBnClickedOpen();
  afx_msg void OnBnClickedClose();
//  afx_msg void OnDropFiles(HDROP hDropInfo);
};
