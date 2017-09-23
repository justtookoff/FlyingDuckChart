//Cut.h
#ifndef _CUT_H
#define _CUT_H
#include "KeyBoardAction.h"

class Cut :public KeyBoardAction{

public:
	Cut();
	Cut(const Cut& source);
	~Cut();
	virtual void CutFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure, NSChart *nsChart);
	virtual void CutGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure, NSChart *nsChart);
	Cut& operator=(const Cut& source);
};
#endif //_CUT_H