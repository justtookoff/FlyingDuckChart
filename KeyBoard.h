//KeyBoard.h
#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include "KeyBoardAction.h"

typedef signed long int Long;

class SelectedStructure;
class CopyStructure;
class NSChart;
class GroupStructure;
class GroupSelectedStructure;
class GroupCopyStructure;
class GroupIndexes;

class KeyBoard{

public:

	KeyBoard();
	KeyBoard(const KeyBoard& source);
	KeyBoard(KeyBoardAction *keyBoardAction);
	~KeyBoard();
	void SetKeyBoardAction(KeyBoardAction *keyBoardAction);
	virtual void CopyFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure);
	virtual void CopyGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure);
	virtual void PasteFunction(CopyStructure *copyStructure, Long x, Long y, NSChart *nsChart);
	virtual void PasteGroupFunction(GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart);
	virtual void CombinedPasteFunction(CopyStructure *copyStructure, GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart);
	virtual void Clear(CopyStructure *copyStructure);
	virtual void DeleteFunction(SelectedStructure *selectdStructure, NSChart *nsChart);
	virtual void DeleteGroupFunction(GroupIndexes *groupIndexes, GroupSelectedStructure *groupSelectedStructure, GroupStructure *groupStructure, NSChart *nsChart);
	virtual void CutFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure, NSChart *nsChart);
	virtual void CutGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure, NSChart *nsChart);
	KeyBoard& operator=(const KeyBoard& source);

private:
	KeyBoardAction *keyBoardAction;

};
#endif _KEYBOARD_H