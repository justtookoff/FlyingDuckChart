//PrintPreview.h
#ifndef _PRINTPREVIEW_H
#define _PRINTPREVIEW_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;

class NSChart;
class FlyingDuckChart;
class PrintPreviewToolbar;
class PrintPreviewScrollBar;
class PrintPreview : public CFrameWnd{

public:
	PrintPreview();
	~PrintPreview();

public:
	void CloseWindow();
	void Print();

public:
	//부모
	FlyingDuckChart *hFlyingDuckChart;

	NSChart *nsChart;
	//자식
	PrintPreviewToolbar *printPreviewToolbar;
	PrintPreviewScrollBar *printPreviewScrollBar;
	Long printPreviewZoom;

private:

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
#endif //_PRINTPREVIEW_H