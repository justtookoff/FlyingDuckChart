//NSChartFormScrollBar.h
#ifndef _NSCHARTFORMSCROLLBAR_H
#define _NSCHARTFORMSCROLLBAR_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;
class FlyingDuckChart;
class NSChartForm;
class NSChartFormScrollBar : public CWnd{

public:
	NSChartFormScrollBar();
	~NSChartFormScrollBar();

public:
	//부모
	FlyingDuckChart *flyingDuckChart;
	//자식
	NSChartForm *nsChartForm;

private:
	//스크롤을 위한 변수
	Long cellWidth;
	Long cellHeight;
	Long ribbonWidth;
	Long viewWidth;
	Long viewHeight;
	Long hScrollPos;
	Long vScrollPos;
	Long hPageSize;
	Long vPageSize;


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	///////////////////////////////////////////////////////////////////////
	//스크롤을 위한 메세지
	afx_msg void OnSize(UINT type, int x, int y);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	////////////////////////////////////////////////////////////////////////
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
#endif //_NSCHAARTFORMSCROLLVIEW_H