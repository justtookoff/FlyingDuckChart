//PickStructure.h

#ifndef _PICKSTRUCTURE_H
#define _PICKSTRUCTURE_H

#include "MouseLButtonAction.h"

class Structure;
class SelectedStructure;
class GroupSelectedStructure;
class Group;

class PickStructure : public MouseLButtonAction {
public :
	PickStructure();
	PickStructure(const PickStructure& source);
	~PickStructure();
	PickStructure& operator=(const PickStructure& source);
	virtual void Pick(NSChart *nsChart, SelectedStructure *selectedStructure, Structure* *pickStructure);
	virtual void PickGroup(NSChart *nsChart, GroupSelectedStructure *groupSelectedStructure, Group * *pickGroupStructure);


};
#endif // !_PICKSTRUCTURE_H
