//Cut.cpp
#include "Cut.h"
#include "SelectedStructure.h"
#include "CopyStructure.h"
#include "NSChart.h"
#include "Collector.h"
#include "Mover.h"
#include "GroupSelectedStructure.h"
#include "GroupCopyStructure.h"
#include "Group.h"

Cut::Cut(){

}

Cut::Cut(const Cut& source){

}

Cut::~Cut(){

}

void Cut::CutFunction(SelectedStructure *selectedStructure, CopyStructure *copyStructure, NSChart *nsChart){
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long index;
	Long x;
	Long y;
	Long width;
	Long height;
	Structure *structure;
	Structure *childStructure;
	Structure *parentStructure;
	Visitor *visitor = new Collector;
	Visitor *visitorMover = new Mover;

	//ī���� �͵��� �ʱ�ȭ
	copyStructure->Clear();
	nsChart->Arrange();

	while (i < selectedStructure->GetLength()){
		parentStructure = selectedStructure->GetAt(i)->GetParentStructure();
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
	
	i = 0;
	selectedStructure->Arrange();

	while (i < selectedStructure->GetLength()){
		parentStructure = selectedStructure->GetAt(i)->GetParentStructure();
		structure = selectedStructure->GetAt(i);
		index = parentStructure->Search(structure);
		x = structure->GetX();
		y = structure->GetY();
		width = structure->GetWidth();
		height = structure->GetHeight();


		//�θ� ������ �� �ڽĵ��� ������ �̵������ش�.
		if (dynamic_cast<Block*>(structure)){
			
			j = 0;
			while (j != dynamic_cast<Block*>(structure)->GetLength()){
				dynamic_cast<Block*>(structure)->GetChild(j)->Accept(visitor, nsChart, structure);

			}
			
		}
	
		if (parentStructure->GetChild(index) != 0){
			delete parentStructure->GetChild(index);
			parentStructure->Delete(index);
		}

		//���� �̵������ش�.
		while (index < dynamic_cast<Block*>(parentStructure)->GetLength()){

			structure = dynamic_cast<Block*>(parentStructure)->GetChild(index);

			if (structure->GetX() == x && structure->GetWidth() == width && structure->GetY() > y){
				structure->Accept(visitorMover, 0, -height);
			}
			index++;
		}

		i++;
	}
	selectedStructure->Clear();

	if (visitor != 0){
		delete visitor;
	}

	if (visitorMover != 0){
		delete visitorMover;
	}
}

void Cut::CutGroupFunction(GroupSelectedStructure *groupSelectedStructure, GroupCopyStructure *groupCopyStructure, NSChart *nsChart) {
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long index;
	Long parentIndex;
	Long x;
	Long y;
	Long width;
	Long height;
	Structure *structure;
	Structure *childStructure;
	Structure *parentStructure;
	Structure *cloneStructure;
	Visitor *visitor = new Collector;
	Visitor *visitorMover = new Mover;
	Group group2;

	//ī���� �͵��� �ʱ�ȭ
	groupCopyStructure->Clear();
	nsChart->Arrange();

	while (i < groupSelectedStructure->GetLength()) {
		Group realGroup;//groupCopyStructure�� �߰��Ǵ� �׷�
		Long v = 0;
		Group *group = groupSelectedStructure->GetAt(i);
		while (v < group->GetLength()) {
			group2.Add(group->GetAt(v));
			parentStructure = group->GetAt(v)->GetParentStructure();
			parentIndex = group->Search(parentStructure);
			if (parentIndex == -1) {
				//���縦 ���Ѵ�.
				cloneStructure = group->GetAt(v)->Clone();
				cloneStructure->SetParent(cloneStructure);
				k = realGroup.Add(cloneStructure);

				//������ �� ������ �ڽĵ��� ���ش�.
				if (dynamic_cast<Block *>(realGroup.GetAt(k))) {
					while (j != dynamic_cast<Block *>(realGroup.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(realGroup.GetAt(k))->GetChild(j);
						delete childStructure;
						dynamic_cast<Block *>(realGroup.GetAt(k))->Delete(j);
					}
				}
			}
			else {
				//���縦 ���Ѵ�.
				cloneStructure = group->GetAt(v)->Clone();
				cloneStructure->SetParent(realGroup.GetAt(parentIndex));
				k = realGroup.Add(cloneStructure);

				//������ �� ������ �ڽĵ��� ���ش�.
				if (dynamic_cast<Block *>(realGroup.GetAt(k))) {
					while (j != dynamic_cast<Block *>(realGroup.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(realGroup.GetAt(k))->GetChild(j);
						delete childStructure;
						dynamic_cast<Block *>(realGroup.GetAt(k))->Delete(j);
					}
				}

				//�θ�� �ڽİ��� ���踦 �����Ѵ�.
				realGroup.GetAt(parentIndex)->Add(realGroup.GetAt(k));
			}
			v++;
		}
		groupCopyStructure->Add(realGroup);
		i++;
	}

	i = 0;
	group2.ReverseArrange();

	while (i < group2.GetLength()) {
		parentStructure = group2.GetAt(i)->GetParentStructure();
		structure = group2.GetAt(i);
		index = parentStructure->Search(structure);
		x = structure->GetX();
		y = structure->GetY();
		width = structure->GetWidth();
		height = structure->GetHeight();

		//�θ� ������ �� �ڽĵ��� ������ �̵������ش�.
		if (dynamic_cast<Block*>(structure)) {

			j = 0;
			while (j != dynamic_cast<Block*>(structure)->GetLength()) {
				dynamic_cast<Block*>(structure)->GetChild(j)->Accept(visitor, nsChart, structure);

			}

		}

		//�θ�κ��� �ڱ��ڽ��� �����.
		if (parentStructure->GetChild(index) != 0) {
			delete parentStructure->GetChild(index);
			parentStructure->Delete(index);
		}

		//���� �̵������ش�.
		while (index < dynamic_cast<Block*>(parentStructure)->GetLength()) {

			structure = dynamic_cast<Block*>(parentStructure)->GetChild(index);

			if (structure->GetX() == x && structure->GetWidth() == width && structure->GetY() > y) {
				structure->Accept(visitorMover, 0, -height);
			}
			index++;
		}

		i++;
	}
	groupSelectedStructure->Clear();

	if (visitor != 0) {
		delete visitor;
	}

	if (visitorMover != 0) {
		delete visitorMover;
	}
}


Cut& Cut::operator=(const Cut& source){

	return *this;
}