//CtrlSelectStructure.h
#ifndef _CTRLSELECTSTRUCTURE_H
#define _CTRLSELECTSTURCUTRE_H

#include "MouseLButtonAction.h"

class CtrlSelectStructure :public MouseLButtonAction{

public:
	CtrlSelectStructure();
	CtrlSelectStructure(const CtrlSelectStructure& source);
	~CtrlSelectStructure();
	virtual void Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result);
	virtual void SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure);
	CtrlSelectStructure& operator=(const CtrlSelectStructure& source);

};

#endif //_CTRLSELECTSTRUCTURE_H