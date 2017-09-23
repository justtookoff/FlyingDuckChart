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

	//structure가 존재하면,
	if (structure != 0) {
		//structure가 GroupStructure에 있는지 확인한다.
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

		//GroupStructure에 있으면,
		if (groupIndex != -1) {
			
			//원래 GroupStructure에 속해 있었는지 확인한다.
			index = groupSelectedStructure->Search(&groupStructure->GetAt(groupIndex));

			//원래 GroupStructure에 속해 있었던 애가 아니면,
			if (index == -1) {
				//GroupSelectedStructure에 더한다.
				groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
			}
			//원래 GroupStructure에 속해 있었으면,
			else {
				//GroupSelectedStructure에서 지운다.
				groupSelectedStructure->Delete(index);
			}
		}
		//GroupStructure에 없으면,
		else {
			*inGroupStructure = 0;
		}
	}
	//structure가 존재하지 않으면,
	else {
		*inGroupStructure = 0;
	}
		
}

CtrlSelectStructure& CtrlSelectStructure::operator=(const CtrlSelectStructure& source){

	return *this;
}