//PrintPreviewToolbar.h
#ifndef _PRINTPREVIEWTOOLBAR_H
#define _PRINTPREVIEWTOOLBAR_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;
class PrintPreview;
class PrintPreviewToolbar : public CWnd{

public:
	PrintPreviewToolbar();
	~PrintPreviewToolbar();
	void DrawPrintPreviewToolbar(CDC *pDC);

public:
	PrintPreview *printPreview;
	CButton printButton;
	CButton zoomInButton;
	CButton zoomOutButton;
	CButton exitButton;

	Long zoom;

private:

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnPrintToolbarClicked();
	afx_msg void OnZoomInToolbarClicked();
	afx_msg void OnZoomOutToolbarClicked();
	afx_msg void OnExitToolbarClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
#endif //_PRINTPREVIEWTOOLBAR_H