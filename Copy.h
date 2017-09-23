//Copy.h
#ifndef _COPY_H
#define _COPY_H
#include "KeyBoardAction.h"

class Copy:public KeyBoardAction{

public:
	Copy();
	Copy(const Copy& source);
	~Copy();
	virtual void CopyFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure);
	virtual void CopyGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure);
	Copy& operator=(const Copy& source);
};

#endif //_COPY_H