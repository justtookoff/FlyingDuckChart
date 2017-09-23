//HelpDialog.h

#ifndef _HELPDIALOG_H
#define _HELPDIALOG_H

#include <afxwin.h>
#include "resource.h"
#include <stdlib.h>
typedef signed long int Long;

class HelpDialog :public CDialog{

public:
	enum{ IDD = IDD_DIALOG1 };

public:
	HelpDialog(CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
public:
	
public:
	Long count;


protected:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};

#endif //_HELPDIALOG_H