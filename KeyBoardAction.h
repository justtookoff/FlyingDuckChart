//KeyBoardAction.h
#ifndef _KEYBOARDACTION_H
#define _KEYBOARDACTION_H


typedef signed long int Long;

class SelectedStructure;
class CopyStructure;
class NSChart;
class GroupSelectedStructure;
class GroupStructure;
class GroupCopyStructure;
class GroupIndexes;

class KeyBoardAction{

public:

	KeyBoardAction();
	KeyBoardAction(const KeyBoardAction& source);
	virtual ~KeyBoardAction() = 0;
	virtual void CopyFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure){}
	virtual void CopyGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure){}
	virtual void PasteFunction(CopyStructure *copyStructure, Long x, Long y, NSChart *nsChart){}
	virtual void PasteGroupFunction(GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart){}
	virtual void CombinedPasteFunction(CopyStructure *copyStructure, GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart){}
	virtual void Clear(CopyStructure *copyStructure){}
	virtual void DeleteFunction(SelectedStructure *selectedStructure, NSChart *nsChart){}
	virtual void DeleteGroupFunction(GroupIndexes *groupIndexes, GroupSelectedStructure *groupSelectedStructure, GroupStructure *groupStructure, NSChart *nsChart){}
	//5.25 부모와의 관계 없음
	virtual void CutFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure, NSChart *nsChart){}
	virtual void CutGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure, NSChart *nsChart){}
	KeyBoardAction& operator=(const KeyBoardAction& source);
};
#endif //_KEYBOARDACTION_H