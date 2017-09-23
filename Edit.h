//Edit.h
#ifndef _EDIT_H
#define _EDIT_H
#include <afxwin.h>
#include <string>
using namespace std;
typedef signed long int Long;

class Text;
class Structure;
class NSChartForm;
class Edit : public CWnd {

public:

	Edit();
	~Edit();
	BOOL IsDBCS(Long index);
	void OnClose();

public:

	Text *text;
	//리스트박스 필요변수들
	CListBox *cListBox;
	Long cListBoxCurrent;
	string *indexes;
	Long sameWordCount;
	Long *sameWordIndexes;
	Long count;

private:

	Long x;
	Long y;
	Long current;
	Long startX;
	Long startY;
	Long startIndex;

	char(*copyString);
	bool isClicked;
	bool startCopy;

	bool startMakeBlock;
	bool bComp;

	Long checkingPoint;
	Structure *structure;


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUP(UINT nChar, UINT nRepCnet, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg LRESULT OnHangeulChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHangeulComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHangeulStartComposition(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

#endif // !_EDIT_H
