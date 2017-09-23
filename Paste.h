//Paste.h
#ifndef _PASTE_H
#define _PASTE_H
#include "KeyBoardAction.h"

class Paste :public KeyBoardAction{

public:
	Paste();
	Paste(const Paste& source);
	~Paste();
	virtual void PasteFunction(CopyStructure *copyStructure, Long x, Long y, NSChart *nsChart);
	virtual void PasteGroupFunction(GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart);
	virtual void CombinedPasteFunction(CopyStructure *copyStructure, GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart);
	Paste& operator=(const Paste& source);

};

#endif //_PASTE_H