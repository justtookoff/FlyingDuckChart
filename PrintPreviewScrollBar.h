//PrintPreviewScrollBar.h
#ifndef _PRINTPREVIEWSCROLLBAR_H
#define _PRINTPREVIEWSCROLLBAR_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;
class PrintPreview;
class PrintPreviewBitmap;
class NSChart;
class PrintPreviewScrollBar : public CWnd{

public:
	PrintPreviewScrollBar();
	~PrintPreviewScrollBar();

public:
	//�θ�
	PrintPreview *printPreview;
	//�ڽ�
	PrintPreviewBitmap *printPreviewBitmap;
	//PrintPreview���� ��������� NSChart�� PrintPreviewBitmap���� �����ֱ����ؼ�
	NSChart *transferNSChart;
	Long transferZoom;

private:
	//��ũ���� ���� ����
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
	//��ũ���� ���� �޼���
	afx_msg void OnSize(UINT type, int x, int y);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	////////////////////////////////////////////////////////////////////////
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
#endif //_PRINTPREVIEWSCROLLBAR_H