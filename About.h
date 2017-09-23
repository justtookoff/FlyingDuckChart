//About.h
#ifndef _ABOUT_H
#define _ABOUT_H
#include <afxwin.h>
#include "resource.h"

typedef signed long int Long;

class About :public CDialog{

public:
	enum { IDD = IDD_DIALOG_ABOUT};

public:
	About(CWnd *parent = NULL);
	virtual BOOL OnInitDialog();

public:

protected:
	afx_msg void OnPaint();
	afx_msg void OnConfirmButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedStaticVersion();
};
#endif //_ABOUT_H