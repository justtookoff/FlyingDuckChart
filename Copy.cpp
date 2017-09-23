//Copy.cpp
#include "Copy.h"
#include "SelectedStructure.h"
#include "CopyStructure.h"
#include "Structure.h"
#include "Block.h"
#include "GroupSelectedStructure.h"
#include "GroupCopyStructure.h"
#include "Group.h"

Copy::Copy(){

}

Copy::Copy(const Copy& source){

}

Copy::~Copy(){

}

void Copy::CopyFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure){
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Structure *structure;
	Structure *childStructure;

	copyStructure->Clear();
	while (i < selectedStructure->GetLength()){
		structure = selectedStructure->GetAt(i);

		k = copyStructure->Add(structure->Clone());
		
		//Ŭ���� �� Selection, Iteratijon, Case�϶� ������ �ڽĵ��� �����ִ� �۾�
		if (dynamic_cast<Block*>(copyStructure->GetAt(k))){
			while (j != dynamic_cast<Block*>(copyStructure->GetAt(k))->GetLength()){
				childStructure = dynamic_cast<Block*>(copyStructure->GetAt(k))->GetChild(j);
				delete childStructure;
				dynamic_cast<Block*>(copyStructure->GetAt(k))->Delete(j);
			}
		}
		i++;
	}
}

void Copy::CopyGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure) {
	Long i = 0;
	Long k = 0;
	Long v = 0;
	Long parentIndex;
	Long index;
	Structure *parentStructure;
	Structure *childStructure;
	Structure *cloneStructure;

	groupCopyStructure->Clear();

	while (i < groupSelectedStructure->GetLength()) {
		Group group;
		Long j = 0;
		while (j < groupSelectedStructure->GetAt(i)->GetLength()) {
			parentStructure = groupSelectedStructure->GetAt(i)->GetAt(j)->GetParentStructure();
			parentIndex = groupSelectedStructure->GetAt(i)->Search(parentStructure);
			if (parentIndex == -1) {
				//���縦 ���Ѵ�
				cloneStructure = groupSelectedStructure->GetAt(i)->GetAt(j)->Clone();
				cloneStructure->SetParent(cloneStructure);
				k = group.Add(cloneStructure);

				//������ �� ������ �ڽ��� ���ش�.
				if (dynamic_cast<Block *>(group.GetAt(k))) {
					while (v != dynamic_cast<Block *>(group.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(group.GetAt(k))->GetChild(v);
						delete childStructure;
						dynamic_cast<Block *>(group.GetAt(k))->Delete(v);
					}
				}
			}
			else {

				//���縦 ���Ѵ�.
				cloneStructure = groupSelectedStructure->GetAt(i)->GetAt(j)->Clone();
				cloneStructure->SetParent(group.GetAt(parentIndex));
				k = group.Add(cloneStructure);

				//������ �� ������ �ڽ��� ���ش�.
				if (dynamic_cast<Block *>(group.GetAt(k))) {
					while (v != dynamic_cast<Block *>(group.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(group.GetAt(k))->GetChild(v);
						delete childStructure;
						dynamic_cast<Block *>(group.GetAt(k))->Delete(v);
					}
				}

				//�θ�� �ڽİ��� ���踦 �����Ѵ�.
				group.GetAt(parentIndex)->Add(group.GetAt(k));

			}
			j++;
		}
		groupCopyStructure->Add(group);
		i++;

	}

}

Copy& Copy::operator=(const Copy& source){

	return *this;
}