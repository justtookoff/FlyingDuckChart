//PrintPreviewBitmap.h
#ifndef _PRINTPREVIEWBITMAP_H
#define _PRINTPREVIEWBITMAP_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;
class PrintPreviewScrollBar;
class Visitor;
class NSChart;
class PrintPreviewBitmap : public CWnd{

public:
	PrintPreviewBitmap();
	~PrintPreviewBitmap();
	void DrawPrintPreviewBitmap(CDC *pDC, CRect rect);

public:
	//스크롤바가 부모임이됨
	PrintPreviewScrollBar *printPreviewScrollBar;
	Visitor *visitor;
	NSChart *printPreviewNSChart;

private:

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
#endif //_PRINTPREVIEWBITMAP_H