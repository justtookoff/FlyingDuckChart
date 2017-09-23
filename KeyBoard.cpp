//KeyBoard.cpp
#include "KeyBoard.h"
#include "Structure.h"

KeyBoard::KeyBoard(){

}

KeyBoard::KeyBoard(const KeyBoard& source)
	:keyBoardAction(source.keyBoardAction){

}

KeyBoard::KeyBoard(KeyBoardAction *keyBoardAction){
	this->keyBoardAction = keyBoardAction;
}

KeyBoard::~KeyBoard(){
	if (this->keyBoardAction != 0){
		delete this->keyBoardAction;
	}
}

void KeyBoard::SetKeyBoardAction(KeyBoardAction *keyBoardAction){

	if (this->keyBoardAction != 0){
		delete this->keyBoardAction;
	}

	this->keyBoardAction = keyBoardAction;

}

void KeyBoard::CopyFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure){

	this->keyBoardAction->CopyFunction(selectedStructure, copyStructure);

}

void KeyBoard::CopyGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure) {

	this->keyBoardAction->CopyGroupFunction(groupSelectedStructure, groupCopyStructure);
}

void KeyBoard::PasteFunction(CopyStructure *copyStructure, Long x, Long y, NSChart *nsChart){

	this->keyBoardAction->PasteFunction(copyStructure, x, y, nsChart);

}

void KeyBoard::PasteGroupFunction(GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart) {

	this->keyBoardAction->PasteGroupFunction(groupCopyStructure, groupStructure, x, y, nsChart);
}

void KeyBoard::CombinedPasteFunction(CopyStructure *copyStructure, GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart) {

	this->keyBoardAction->CombinedPasteFunction(copyStructure, groupCopyStructure, groupStructure, x, y, nsChart);
}

void KeyBoard::Clear(CopyStructure *copyStructure){
	
	this->keyBoardAction->Clear(copyStructure);

}

void KeyBoard::DeleteFunction(SelectedStructure *selectedStructure, NSChart *nsChart){

	this->keyBoardAction->DeleteFunction(selectedStructure,nsChart);
}

void KeyBoard::DeleteGroupFunction(GroupIndexes *groupIndexes, GroupSelectedStructure *groupSelectedStructure, GroupStructure *groupStructure, NSChart *nsChart) {

	this->keyBoardAction->DeleteGroupFunction(groupIndexes, groupSelectedStructure, groupStructure, nsChart);
}

void KeyBoard::CutFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure, NSChart *nsChart){
	this->keyBoardAction->CutFunction(selectedStructure, copyStructure, nsChart);
}

void KeyBoard::CutGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure, NSChart *nsChart) {

	this->keyBoardAction->CutGroupFunction(groupSelectedStructure, groupCopyStructure, nsChart);
}

KeyBoard& KeyBoard::operator=(const KeyBoard& source){

	return *this;
}