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

	//카피할 것들을 초기화
	copyStructure->Clear();
	nsChart->Arrange();

	while (i < selectedStructure->GetLength()){
		parentStructure = selectedStructure->GetAt(i)->GetParentStructure();
		structure = selectedStructure->GetAt(i);
		
		k = copyStructure->Add(structure->Clone());
		
		//클론할 때 Selection, Iteratijon, Case일때 딸려온 자식들을 지워주는 작업
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


		//부모가 지워질 때 자식들을 옆으로 이동시켜준다.
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

		//위로 이동시켜준다.
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

	//카피할 것들을 초기화
	groupCopyStructure->Clear();
	nsChart->Arrange();

	while (i < groupSelectedStructure->GetLength()) {
		Group realGroup;//groupCopyStructure에 추가되는 그룹
		Long v = 0;
		Group *group = groupSelectedStructure->GetAt(i);
		while (v < group->GetLength()) {
			group2.Add(group->GetAt(v));
			parentStructure = group->GetAt(v)->GetParentStructure();
			parentIndex = group->Search(parentStructure);
			if (parentIndex == -1) {
				//복사를 더한다.
				cloneStructure = group->GetAt(v)->Clone();
				cloneStructure->SetParent(cloneStructure);
				k = realGroup.Add(cloneStructure);

				//복사할 때 딸려온 자식들을 없앤다.
				if (dynamic_cast<Block *>(realGroup.GetAt(k))) {
					while (j != dynamic_cast<Block *>(realGroup.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(realGroup.GetAt(k))->GetChild(j);
						delete childStructure;
						dynamic_cast<Block *>(realGroup.GetAt(k))->Delete(j);
					}
				}
			}
			else {
				//복사를 더한다.
				cloneStructure = group->GetAt(v)->Clone();
				cloneStructure->SetParent(realGroup.GetAt(parentIndex));
				k = realGroup.Add(cloneStructure);

				//복사할 때 딸려온 자식들을 없앤다.
				if (dynamic_cast<Block *>(realGroup.GetAt(k))) {
					while (j != dynamic_cast<Block *>(realGroup.GetAt(k))->GetLength()) {
						childStructure = dynamic_cast<Block *>(realGroup.GetAt(k))->GetChild(j);
						delete childStructure;
						dynamic_cast<Block *>(realGroup.GetAt(k))->Delete(j);
					}
				}

				//부모와 자식과의 관계를 설정한다.
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

		//부모가 지워질 때 자식들을 옆으로 이동시켜준다.
		if (dynamic_cast<Block*>(structure)) {

			j = 0;
			while (j != dynamic_cast<Block*>(structure)->GetLength()) {
				dynamic_cast<Block*>(structure)->GetChild(j)->Accept(visitor, nsChart, structure);

			}

		}

		//부모로부터 자기자신을 지운다.
		if (parentStructure->GetChild(index) != 0) {
			delete parentStructure->GetChild(index);
			parentStructure->Delete(index);
		}

		//위로 이동시켜준다.
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