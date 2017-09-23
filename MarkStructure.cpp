//MarkStructure.cpp
#include "MarkStructure.h"
#include "NSChart.h"
#include "Structure.h"
#include "CheckingPoint.h"
#include "GroupStructure.h"
#include "Group.h"
#include "SelectedStructure.h"

MarkStructure::MarkStructure(){

}

MarkStructure::MarkStructure(const MarkStructure& source){

}

MarkStructure::~MarkStructure(){

}

void MarkStructure::Mark(NSChart *nsChart, GroupStructure *groupStructure, SelectedStructure *selectedStructure, Long x, Long y, Long *currentState, Long *previousState, Structure* *markStructure, Long *checkingResult){
	CheckingPoint *checkingPoint = 0;
	Long i = 0;
	Long groupIndex = -1;
	Long selectedFlag = 0;

	//05-26
	//도형을 삽입할 위치를 찾을 때
	Structure *structure = nsChart->Find(x, y);

	if (structure != 0){
		//structure가 GroupStructure에 속해 있는지 확인한다.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);

			Long j = 0;
			while (j < group.GetLength()) {
				if (structure == group.GetAt(j)) {
					groupIndex = i;
				}
				j++;
			}
			i++;
		}

		//structure가 selectedStructure에 속해 있는지 확인한다.
		i = 0;
		while (i < selectedStructure->GetLength()) {
			if (structure == selectedStructure->GetAt(i)) {
				selectedFlag++;
			}
			i++;
		}

		//각 도형의 윗변 밑변인지 확인한다
		*checkingResult = checkingPoint->CheckingMarkStructure(structure, x, y);

		if (selectedFlag == 0 && groupIndex == -1 && *checkingResult == 4 || selectedFlag == 0 && groupIndex == -1 && *checkingResult == 6 || selectedFlag == 0 && groupIndex == -1 && *checkingResult == 8 || selectedFlag == 0 && groupIndex == -1 && *checkingResult >= 9){

			*previousState = *currentState;

			*markStructure = structure;
			//표시상태 변경
			*currentState = 1; // 상태변경
	
		}
		else {
			*previousState = *currentState;

			//표시 상태 변경
			*markStructure = 0;

			*currentState = 0; //상태변경
	
		}
	}
	
}


MarkStructure& MarkStructure::operator=(const MarkStructure& source){

	return *this;
}