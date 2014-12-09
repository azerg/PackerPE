#pragma once


// CEditDrop

class CEditDrop : public CEdit
{
  DECLARE_DYNAMIC( CEditDrop )

public:
  CEditDrop();
  virtual ~CEditDrop();

  virtual void OnFinalRelease();
  afx_msg void OnDropFiles( HDROP hDropInfo ); 

protected:
  DECLARE_MESSAGE_MAP()
  DECLARE_DISPATCH_MAP()
  DECLARE_INTERFACE_MAP()
};


