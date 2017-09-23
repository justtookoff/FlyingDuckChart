//JustSelectStructure.h

#ifndef _JUSTSELECTSTRUCTURE_H
#define _JUSTSELECTSTRUCTURE_H
#include "MouseLButtonAction.h"

class JustSelectStructure :public MouseLButtonAction{

public:
	JustSelectStructure();
	JustSelectStructure(const JustSelectStructure& source);
	~JustSelectStructure();
	virtual void Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result);
	virtual void SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure);
	JustSelectStructure& operator=(const JustSelectStructure& source);
	
};

#endif //_JUSTSELECTSTRUCTURE_H