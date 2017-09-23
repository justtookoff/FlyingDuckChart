//FlyingDuckChart.h
#ifndef _FLYINGDUCKCHART_H
#define _FLYINGDUCKCHART_H
#include <afxwin.h>
#include <afxext.h>

typedef signed long int Long;
class Toolbar;
class NSChartFormScrollBar;
class NSChartForm;
class PrintPreview;
class FlyingDuckChart : public CFrameWnd {

public:
	FlyingDuckChart();
	~FlyingDuckChart();

public:
	void NewMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2);
	void OpenMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2);
	void SaveMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2);
	void Print();

public:
	NSChartFormScrollBar *nsChartFormScrollBar;
	Toolbar *toolbar;
	PrintPreview *printPreview;

	CMenu menu;
	Long changedStatus;
	CString fileName;

private:


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//파일
	afx_msg void OnNewMenuClicked();
	afx_msg void OnOpenMenuClicked();
	afx_msg void OnSaveMenuClicked();
	afx_msg void OnAnotherSaveMenuClicked();
	afx_msg void OnPrintMenuClicked();
	afx_msg void OnPrintPreviewMenuClicked();
	afx_msg void OnExitMenuClicked();
	//편집
	afx_msg void OnCutMenuClicked();
	afx_msg void OnCopyMenuClicked();
	afx_msg void OnPasteMenuClicked();
	afx_msg void OnDeleteMenuClicked();
	afx_msg void OnSelectAllMenuClicked();
	afx_msg void OnGroupMenuClicked();
	afx_msg void OnUnLockGroupMenuClicked();
	afx_msg void OnAutoCorrectMenuClicked();
	//기호
	afx_msg void OnSequenceMenuClicked();
	afx_msg void OnSelectionMenuClicked();
	afx_msg void OnIterationMenuClicked();
	afx_msg void OnCaseMenuClicked();
	//도움말
	afx_msg void OnHelpMenuClicked();
	afx_msg void OnInfoMenuClicked();
	//우클릭
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnPopupCutMenuClicked();
	afx_msg void OnPopupCopyMenuClicked();
	afx_msg void OnPopupPasteMenuClicked();
	afx_msg void OnPopupDeleteMenuClicked();
	afx_msg void OnPopupSelectAllMenuClicked();
	afx_msg void OnPopupGroupMenuClicked();
	afx_msg void OnPopupUnLockGroupMenuClicked();
	afx_msg void OnPopupAutoCorrectMenuClicked();

	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

};
#endif //_FLYINGDUCKCHART_H