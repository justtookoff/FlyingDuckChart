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
	//������ ������ ��ġ�� ã�� ��
	Structure *structure = nsChart->Find(x, y);

	if (structure != 0){
		//structure�� GroupStructure�� ���� �ִ��� Ȯ���Ѵ�.
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

		//structure�� selectedStructure�� ���� �ִ��� Ȯ���Ѵ�.
		i = 0;
		while (i < selectedStructure->GetLength()) {
			if (structure == selectedStructure->GetAt(i)) {
				selectedFlag++;
			}
			i++;
		}

		//�� ������ ���� �غ����� Ȯ���Ѵ�
		*checkingResult = checkingPoint->CheckingMarkStructure(structure, x, y);

		if (selectedFlag == 0 && groupIndex == -1 && *checkingResult == 4 || selectedFlag == 0 && groupIndex == -1 && *checkingResult == 6 || selectedFlag == 0 && groupIndex == -1 && *checkingResult == 8 || selectedFlag == 0 && groupIndex == -1 && *checkingResult >= 9){

			*previousState = *currentState;

			*markStructure = structure;
			//ǥ�û��� ����
			*currentState = 1; // ���º���
	
		}
		else {
			*previousState = *currentState;

			//ǥ�� ���� ����
			*markStructure = 0;

			*currentState = 0; //���º���
	
		}
	}
	
}


MarkStructure& MarkStructure::operator=(const MarkStructure& source){

	return *this;
}