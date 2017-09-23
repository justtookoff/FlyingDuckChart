//NSChartForm.h
#ifndef _NSCHARTFORM_H
#define _NSCHARTFORM_H
#include <afxwin.h>
#include <afxext.h>
#include <string>
using namespace std;

typedef signed long int Long;

class NSChartFormScrollBar;
class NSChart;
class Structure;
class SelectedStructure;
class CopyStructure;
class PickStructure;
class Edit;
class GroupStructure;
class GroupSelectedStructure;
class GroupCopyStructure;
class Group;
class GroupIndexes;
class Visitor;

class NSChartForm : public CWnd {

public :

	NSChartForm();
	~NSChartForm();
	void DrawImage(CDC *pDC);
	Long Load(CString str);
	Long Save(CString str);
	void OnClose();
	void EditMenu(UINT nChar);
	void StructureMenu(UINT nChar);
	//클립보드
	void ClipBoardCopy();
	void ClipBoardDrawImage(CDC *pDC);


public :
	NSChartFormScrollBar *nsChartFormScrollBar;
	NSChart *nsChart;
	SelectedStructure *selectedStructure;
	CopyStructure *copyStructure;
	Edit *form;
	GroupStructure *groupStructure;
	GroupSelectedStructure *groupSelectedStructure;
	GroupCopyStructure *groupCopyStructure;
	GroupIndexes *groupIndexes;

	Long checkingResult;
	bool editOpen;
	//캐릭터배열
	char(*editCopy);

private :

	Long currentX;
	Long currentY;
	Long previousX;
	Long previousY;

	CRectTracker rect;
	CPoint startPos;
	CPoint endPos;
	bool startToMove;
	bool startToResize;
	Structure *currentStructure;
	Structure *markStructure;
	Long currentState;
	Long previousState;
	Long result;
	Structure *preview;
	bool previewMove;
	bool drag;
	Structure *pickStructure;
	Group *pickGroupStructure;
	Group *currentGroup;

	Long groupIndex;
	bool groupStructureMove;
	

protected :

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyUP(UINT nChar, UINT nRepCnet, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	DECLARE_MESSAGE_MAP()
};

#endif // !_NSCHARTFORM_H
