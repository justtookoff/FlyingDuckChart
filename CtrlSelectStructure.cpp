//CtrlSelectStructure.cpp

#include "CtrlSelectStructure.h"
#include "SelectedStructure.h"
#include "Structure.h"
#include "GroupStructure.h"
#include "GroupSelectedStructure.h"

CtrlSelectStructure::CtrlSelectStructure(){

}

CtrlSelectStructure::CtrlSelectStructure(const CtrlSelectStructure& source){

}


CtrlSelectStructure::~CtrlSelectStructure(){

}

void CtrlSelectStructure::Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result){
	Long index;
	
	if (structure != 0){
		index = selectedStructure->Search(structure);

		if (index == -1){

			selectedStructure->Add(structure);
		}
		else{
			selectedStructure->Delete(index);
		}

	}

}

void CtrlSelectStructure::SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure) {
	Long i = 0;
	Long groupIndex = -1;
	Long index;

	//structure�� �����ϸ�,
	if (structure != 0) {
		//structure�� GroupStructure�� �ִ��� Ȯ���Ѵ�.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);

			Long j = 0;
			while (j < group.GetLength()) {
				if (structure == group.GetAt(j)) {
					*inGroupStructure = structure;
					groupIndex = i;
				}
				j++;
			}
			i++;
		}

		//GroupStructure�� ������,
		if (groupIndex != -1) {
			
			//���� GroupStructure�� ���� �־����� Ȯ���Ѵ�.
			index = groupSelectedStructure->Search(&groupStructure->GetAt(groupIndex));

			//���� GroupStructure�� ���� �־��� �ְ� �ƴϸ�,
			if (index == -1) {
				//GroupSelectedStructure�� ���Ѵ�.
				groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
			}
			//���� GroupStructure�� ���� �־�����,
			else {
				//GroupSelectedStructure���� �����.
				groupSelectedStructure->Delete(index);
			}
		}
		//GroupStructure�� ������,
		else {
			*inGroupStructure = 0;
		}
	}
	//structure�� �������� ������,
	else {
		*inGroupStructure = 0;
	}
		
}

CtrlSelectStructure& CtrlSelectStructure::operator=(const CtrlSelectStructure& source){

	return *this;
}