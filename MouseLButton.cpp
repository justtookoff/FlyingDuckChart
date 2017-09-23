//MouseLButton.cpp

#include "MouseLButton.h"


MouseLButton::MouseLButton(){

}

MouseLButton::MouseLButton(MouseLButtonAction *mouseLButtonAction){

	this->mouseLButtonAction = mouseLButtonAction;
}

MouseLButton::~MouseLButton(){

	if (this->mouseLButtonAction != 0){
	
		delete this->mouseLButtonAction;
	}
}


void MouseLButton::SetState(MouseLButtonAction *mouseLButtonAction){

	if (this->mouseLButtonAction != 0){
	
		delete this->mouseLButtonAction;
	}
	this->mouseLButtonAction = mouseLButtonAction;
}

void MouseLButton::Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result){

	this->mouseLButtonAction->Select(selectedStructure, structure, x, y, result);
}

void MouseLButton::SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure) {

	this->mouseLButtonAction->SelectGroup(groupStructure, groupSelectedStructure, selectedStructure, structure, inGroupStructure);
}

void MouseLButton::Resize(SelectedStructure *selectedStructure, Long x, Long y, Long checkingResult){

	this->mouseLButtonAction->Resize(selectedStructure, x, y, checkingResult);
}

void MouseLButton::Mark(NSChart *nsChart, GroupStructure *groupStructure, SelectedStructure *selectedStructure, Long x, Long y, Long *currentState, Long *previousState, Structure* *markStructure, Long *checkingResult){

	this->mouseLButtonAction->Mark(nsChart, groupStructure, selectedStructure, x, y, currentState, previousState, markStructure, checkingResult);

}

void MouseLButton::Pick(NSChart *nsChart, SelectedStructure *selectedStructure, Structure* *pickStructure) {

	this->mouseLButtonAction->Pick(nsChart, selectedStructure, pickStructure);

}

void MouseLButton::PickGroup(NSChart *nsChart, GroupSelectedStructure *groupSelectedStructure, Group* *pickGroupStructure) {

	this->mouseLButtonAction->PickGroup(nsChart, groupSelectedStructure, pickGroupStructure);

}

void MouseLButton::Insert(Structure *insertStructure, Structure *markStructure, Long checkingResult){

	this->mouseLButtonAction->Insert(insertStructure, markStructure, checkingResult);

}

void MouseLButton::InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult) {

	this->mouseLButtonAction->InsertGroup(insertGroup, markStructure, checkingResult);
}

MouseLButton& MouseLButton::operator=(const MouseLButton& source){

	return *this;
}