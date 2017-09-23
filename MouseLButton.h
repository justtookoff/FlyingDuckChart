//MouseLButton.h
#ifndef _MOUSELBUTTON_H
#define _MOUSELBUTTON_H
#include "MouseLButtonAction.h"

typedef signed long int Long;

class SelectedStructure;
class Structure;
class NSChart;
class Visitor;
class GroupStructure;
class GroupSelectedStructure;
class Group;

class MouseLButton{

public:
	MouseLButton();
	MouseLButton(MouseLButtonAction *mouseLButtonAction);
	~MouseLButton();
	void SetState(MouseLButtonAction *mouseLButtonAction);
	//선택할 때,
	void Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result);
	void SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure);
	void Resize(SelectedStructure *selectedStructure, Long x, Long y, Long checkingResult);
	//5.26 ns차트와의 관계만함-삽입 위치 찾기
	void Mark(NSChart *nsChart, GroupStructure *groupStructure, SelectedStructure *selectedStructure, Long x, Long y, Long *currentState, Long *previousState, Structure* *markStructure, Long *checkingResult);
	//새로운 기호를 뺄때,
	void Pick(NSChart *nsChart, SelectedStructure *selectedStructure, Structure* *pickStructure);
	void PickGroup(NSChart *nsChart, GroupSelectedStructure *groupSelectedStructure, Group* *pickGroupStructure);
	//삽입할 때
	void Insert(Structure *insertStructure, Structure *markStructure, Long checkingResult);
	void InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult);
	MouseLButton& operator=(const MouseLButton& source);

private:
	MouseLButtonAction *mouseLButtonAction;
};

#endif //_MOUSELBUTTON_H