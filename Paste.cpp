//Paste.cpp
#include "Paste.h"
#include "Structure.h"
#include "CopyStructure.h"
#include "NSChart.h"
#include "GroupCopyStructure.h"
#include "GroupStructure.h"
#include "Group.h"

Paste::Paste(){

}

Paste::Paste(const Paste& source){

}

Paste::~Paste(){

}

void Paste::PasteFunction(CopyStructure *copyStructure, Long x, Long y, NSChart *nsChart){
	Long i = 0;
	Long index;
	Long previousX = x;
	Long previousY = y;
	Long mX = 0;
	Long mY = 0;
	Structure *structure;

	copyStructure->Arrange();

	while (i < copyStructure->GetLength()){
		
		structure = copyStructure->GetAt(i)->Clone();

		if (i > 0){
			mX = copyStructure->GetAt(i - 1)->GetX() - copyStructure->GetAt(i)->GetX();
			mY = copyStructure->GetAt(i - 1)->GetY() - copyStructure->GetAt(i)->GetY();
		}

		structure->Correct(previousX - mX - structure->GetX(), previousY - mY - structure->GetY());
		
		//부모설정
		structure->SetParent(nsChart);

		index = nsChart->Add(structure);

		previousX = nsChart->GetChild(index)->GetX();
		previousY = nsChart->GetChild(index)->GetY();
	
		i++;
	}
}

void Paste::PasteGroupFunction(GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart) {
	Long i = 0;
	Long v = 0;
	Long k = 0;
	Structure *structure;
	Structure *parentStructure;
	Structure *childStructure;
	Structure *cloneStructure;
	Group group2;
	Group group3;

	while (i < groupCopyStructure->GetLength()) {
		Group group;
		Long j = 0;
		Long index;
		Long parentIndex;

		while (j < groupCopyStructure->GetAt(i).GetLength()) {
			structure = groupCopyStructure->GetAt(i).GetAt(j);

			//복제한다.
			cloneStructure = structure->Clone();

			//복제할 때 딸려온 자식을 지운다.
			if (dynamic_cast<Block *>(cloneStructure)) {
				while (v != dynamic_cast<Block *>(cloneStructure)->GetLength()) {
					childStructure = dynamic_cast<Block *>(cloneStructure)->GetChild(v);
					delete childStructure;
					dynamic_cast<Block *>(cloneStructure)->Delete(v);
				}
			}

			//부모설정
			if (structure->GetParentStructure() == structure) {
				cloneStructure->SetParent(nsChart);

				index = nsChart->Add(cloneStructure);
				group.Add(cloneStructure);

			}
			else {
				parentStructure = structure->GetParentStructure();
				parentIndex = groupCopyStructure->GetAt(i).Search(parentStructure);
				cloneStructure->SetParent(group.GetAt(parentIndex));
				k = group.Add(cloneStructure);
				group.GetAt(parentIndex)->Add(group.GetAt(k));

			}

			group2.Add(cloneStructure);
			group3.Add(cloneStructure->Clone());
			j++;
		}
		groupStructure->Add(group);
		i++;
	}

	//Correct 해준다.
	i = 0;
	Long previousX = x;
	Long previousY = y;
	Long mX = 0;
	Long mY = 0;

	group2.Arrange();
	group3.Arrange();
	while (i < group2.GetLength()) {
		structure = group2.GetAt(i);

		if (i > 0) {
			mX = group3.GetAt(i - 1)->GetX() - group3.GetAt(i)->GetX();
			mY = group3.GetAt(i - 1)->GetY() - group3.GetAt(i)->GetY();
		}

		structure->Correct(previousX - mX - structure->GetX(), previousY - mY - structure->GetY());

		previousX = structure->GetX();
		previousY = structure->GetY();
		i++;
	}

	i = 0;
	while (i < group3.GetLength()) {
		delete group3.GetAt(i);
		i++;
	}

}

void Paste::CombinedPasteFunction(CopyStructure *copyStructure, GroupCopyStructure *groupCopyStructure, GroupStructure *groupStructure, Long x, Long y, NSChart *nsChart) {
	Long i = 0;
	Long previousX = x;
	Long previousY = y;
	Long mX = 0;
	Long mY = 0;
	Long v = 0;
	Long k = 0;
	Structure *structure;
	Structure *parentStructure;
	Structure *childStructure;
	Structure *cloneStructure;
	Group group2;
	Group group3;

	while (i < copyStructure->GetLength()) {

		structure = copyStructure->GetAt(i)->Clone();
		
		//부모설정
		structure->SetParent(nsChart);

		nsChart->Add(structure);
		group2.Add(structure);
		group3.Add(structure->Clone());
				
		i++;
	}

	i = 0;
	while (i < groupCopyStructure->GetLength()) {
		Group group;
		Long j = 0;
		Long index;
		Long parentIndex;

		while (j < groupCopyStructure->GetAt(i).GetLength()) {
			structure = groupCopyStructure->GetAt(i).GetAt(j);

			//복제한다.
			cloneStructure = structure->Clone();

			//복제할 때 딸려온 자식을 지운다.
			if (dynamic_cast<Block *>(cloneStructure)) {
				while (v != dynamic_cast<Block *>(cloneStructure)->GetLength()) {
					childStructure = dynamic_cast<Block *>(cloneStructure)->GetChild(v);
					delete childStructure;
					dynamic_cast<Block *>(cloneStructure)->Delete(v);
				}
			}

			//부모설정
			if (structure->GetParentStructure() == structure) {
				cloneStructure->SetParent(nsChart);

				index = nsChart->Add(cloneStructure);
				group.Add(cloneStructure);
				

			}
			else {
				parentStructure = structure->GetParentStructure();
				parentIndex = groupCopyStructure->GetAt(i).Search(parentStructure);
				cloneStructure->SetParent(group.GetAt(parentIndex));
				k = group.Add(cloneStructure);
				group.GetAt(parentIndex)->Add(group.GetAt(k));

			}

			group2.Add(cloneStructure);
			group3.Add(cloneStructure->Clone());
			j++;
		}
		groupStructure->Add(group);
		i++;
	}

	//Correct 한다.
	i = 0;
	
	group2.Arrange();
	group3.Arrange();
	while (i < group2.GetLength()) {
		structure = group2.GetAt(i);

		if (i > 0) {
			mX = group3.GetAt(i - 1)->GetX() - group3.GetAt(i)->GetX();
			mY = group3.GetAt(i - 1)->GetY() - group3.GetAt(i)->GetY();
		}

		structure->Correct(previousX - mX - structure->GetX(), previousY - mY - structure->GetY());

		previousX = structure->GetX();
		previousY = structure->GetY();
		i++;
	}

	i = 0;
	while (i < group3.GetLength()) {
		delete group3.GetAt(i);
		i++;
	}
}

Paste& Paste::operator=(const Paste& source){

	return *this;
}
