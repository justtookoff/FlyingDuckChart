//MouseLButtonAction.h
#ifndef _MOUSELBUTTONACTION_H
#define _MOUSELBUTTONACTION_H

typedef signed long int Long;

class SelectedStructure;
class Structure;
class NSChart;
class Visitor;
class GroupSelectedStructure;
class GroupStructure;
class Group;

class MouseLButtonAction{

public:
	MouseLButtonAction();
	MouseLButtonAction(const MouseLButtonAction& source);
	virtual~MouseLButtonAction() = 0;
	virtual void Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result){}
	virtual void SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure) {}
	virtual void Resize(SelectedStructure *selectedStructure, Long x, Long y, Long checkingResult){}
	virtual void Mark(NSChart *nsChart, GroupStructure *groupStructure, SelectedStructure *selectedStructure, Long x, Long y, Long *currentState, Long *previousState, Structure* *markStructure, Long *checkingResult){}
	virtual void Pick(NSChart *nsChart, SelectedStructure *selectedStructure, Structure* *pickStructure){}
	virtual void PickGroup(NSChart *nsChart, GroupSelectedStructure *groupSelectedStructure, Group* *pickGroupStructure){}
	virtual void Insert(Structure *insertStructure, Structure *markStructure, Long checkingResult){}
	virtual void InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult){}
	
	MouseLButtonAction& operator =(const MouseLButtonAction& source);

};


#endif //_STATE_H