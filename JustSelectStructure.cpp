//JustSelecStructure.cpp

#include "JustSelectStructure.h"
#include "SelectedStructure.h"
#include "Structure.h"
#include "CheckingPoint.h"
#include "GroupStructure.h"
#include "GroupSelectedStructure.h"
#include "SelectedStructure.h"

JustSelectStructure::JustSelectStructure(){

}

JustSelectStructure::JustSelectStructure(const JustSelectStructure& source){

}



JustSelectStructure::~JustSelectStructure(){

}

void JustSelectStructure::Select(SelectedStructure *selectedStructure, Structure *structure, Long x, Long y, Long *result){
	Long index;
	CheckingPoint checkingPoint;
	Long i = 0;
	Long result2;
	Long flag = 0;

	if (structure != 0){

		if (selectedStructure->GetLength() != 0){

			i = 0;
			flag = 0;
			while (i < selectedStructure->GetLength() && flag == 0){

				result2 = checkingPoint.CheckingResult(selectedStructure->GetAt(i), x, y);

				if (result2 != -1){

					flag++;
				}
				i++;
			}

			if (result2 == -1){
				index = selectedStructure->Search(structure);

				if (index == -1){

					selectedStructure->Clear();
					selectedStructure->Add(structure);
				}


			}
			else{

				*result = result2;
			}

		}
		else{
			selectedStructure->Add(structure);
		}
	}
	else{
		if (selectedStructure->GetLength() != 0){
			i = 0;
			flag = 0;
			while (i < selectedStructure->GetLength() && flag == 0){

				result2 = checkingPoint.CheckingResult(selectedStructure->GetAt(i), x, y);

				if (result2 != -1){

					flag++;
				}
				i++;
			}

			if (result2 == -1){
				selectedStructure->Clear();
			}
			else{

				*result = result2;
			}


		}
	}

}

void JustSelectStructure::SelectGroup(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Structure *structure, Structure* *inGroupStructure) {
	Long i = 0;
	Long groupIndex = -1;
	Long index;
	
	//structure�� �����ϸ�
	if (structure != 0) {
		
		//structure�� GroupStructure�� ���� �ִ��� Ȯ���Ѵ�.
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

		//structure�� GroupStructure�� ���� ������,
		if (groupIndex != -1) {
			
			//���� GroupStructure�� ���� �־����� Ȯ���Ѵ�.
			index = groupSelectedStructure->Search(&groupStructure->GetAt(groupIndex));

			//���� GroupStructure�� ���� �־��� �ְ� �ƴϸ�,
			if (index == -1) {
				
				selectedStructure->Clear();
				groupSelectedStructure->Clear();
				groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
			}
						
		}
		//GroupStructure�� ���� ���� ������,
		else {
			
			//structure�� selectedStructure�� ���� �ִ��� Ȯ���Ѵ�.
			index = selectedStructure->Search(structure);
			
			//selectedStructure�� ���� ���� ������,
			if (index == -1) {
				groupSelectedStructure->Clear();
				*inGroupStructure = 0;
			}
			//selectedStructure�� ���� ������,
			else {
				*inGroupStructure = 0;
			}
			
			
		}
	}
	//structure�� �������� ������,
	else {
		groupSelectedStructure->Clear();
		*inGroupStructure = 0;
	}

}

JustSelectStructure& JustSelectStructure::operator=(const JustSelectStructure& source){

	return *this;
}