//Delete.h
#ifndef _DELETE_H
#define _DELETE_H
#include "KeyBoardAction.h"

class SelectedStructure;
class NSChart;
class CopyStructure;
class GroupStructure;
class GroupSelectedStructure;
class GroupIndexes;

class Delete :public KeyBoardAction{

public:

	Delete();
	Delete(const Delete& source);
	~Delete();
	void DeleteFunction(SelectedStructure *selectedStructure, NSChart *nsChart);
	void DeleteGroupFunction(GroupIndexes *groupIndexes, GroupSelectedStructure *groupSelectedStructure, GroupStructure *groupStructure, NSChart *nsChart);
	Delete& operator=(const Delete& source);

};
#endif //_DELETE_H