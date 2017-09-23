//PickStructure.cpp

#include "PickStructure.h"
#include "SelectedStructure.h"
#include "Structure.h"
#include "Block.h"
#include "Visitor.h"
#include "Collector.h"
#include "Mover.h"
#include "Group.h"
#include "GroupSelectedStructure.h"
#include "NSChart.h"

PickStructure::PickStructure() {

}

PickStructure::PickStructure(const PickStructure& source) {

}

PickStructure::~PickStructure() {

}

PickStructure& PickStructure::operator=(const PickStructure& source) {
	return *this;
}

void PickStructure::Pick(NSChart *nsChart, SelectedStructure *selectedStructure, Structure* *pickStructure) {
	
	*pickStructure = selectedStructure->GetAt(0);
	Long x = (*pickStructure)->GetX();
	Long y = (*pickStructure)->GetY();
	Long width = (*pickStructure)->GetWidth();
	Long height = (*pickStructure)->GetHeight();

	Visitor *visitor = new Collector;
	Visitor *visitor2 = new Mover;

	//자식들을 collector로 옆으로 이동시킨다.
	if (dynamic_cast<Block *>(*pickStructure)) {
		Long q = 0;
		while (q != dynamic_cast<Block *>(*pickStructure)->GetLength()) {
			Structure *childStructure = dynamic_cast<Block *>(*pickStructure)->GetChild(q);
			childStructure->Accept(visitor, nsChart, *pickStructure);
		}
	}

	Structure *parent = (*pickStructure)->GetParentStructure();

	if (parent != *pickStructure) {
		Long index = parent->Search(*pickStructure);
		parent->Delete(index);
		(*pickStructure)->SetParent(*pickStructure);

		while (index < dynamic_cast<Block *>(parent)->GetLength()) {
			Structure *childStructure2 = dynamic_cast<Block *>(parent)->GetChild(index);
			if (childStructure2->GetX() == x && childStructure2->GetWidth() == width && childStructure2->GetY() > y) {
				childStructure2->Accept(visitor2, 0, -height);
			}
			index++;
		}
	}

	if (visitor != NULL) {
		delete visitor;
	}

	if (visitor2 != NULL) {
		delete visitor2;
	}

}

void PickStructure::PickGroup(NSChart *nsChart, GroupSelectedStructure *groupSelectedStructure, Group* *pickGroupStructure){
	Long i = 0;
	Visitor *visitor = new Mover;
	Visitor *visitorCollector = new Collector;

	nsChart->Arrange();

	*pickGroupStructure = groupSelectedStructure->GetAt(0);
	(*pickGroupStructure)->ReverseArrange();

	while (i < (*pickGroupStructure)->GetLength()) {
		Structure *groupStructure = (*pickGroupStructure)->GetAt(i);
		Long x = (*pickGroupStructure)->GetAt(i)->GetX();
		Long y = (*pickGroupStructure)->GetAt(i)->GetY();
		Long width = (*pickGroupStructure)->GetAt(i)->GetWidth();
		Long height = (*pickGroupStructure)->GetAt(i)->GetHeight();

		//자식들을 collector로 옆으로 이동시킨다.
		if (dynamic_cast<Block *>(groupStructure)) {
			Long q = 0;
			while (q < dynamic_cast<Block *>((*pickGroupStructure)->GetAt(i))->GetLength()) {
				
				Structure *childStructure = dynamic_cast<Block *>((*pickGroupStructure)->GetAt(i))->GetChild(q);
				//자식이 GroupStructure에 속해 있는지 확인한다.
				Long childIndex = groupSelectedStructure->GetAt(0)->Search(childStructure);
				if (childIndex == -1){
					childStructure->Accept(visitorCollector, nsChart, (*pickGroupStructure)->GetAt(i));
					//childStructure->SetParent((*pickGroupStructure)->GetAt(i));
				}
				else{
					q++;
				}
			}
		}

		//그룹화된 스트럭쳐들 중에 하나의 스트럭쳐의 부모를 찾는다.
		Structure *parent = (*pickGroupStructure)->GetAt(i)->GetParentStructure();

		//그 중에 몇번째인지 찾는다.
		Long parentIndex = groupSelectedStructure->GetAt(0)->Search(parent);

		//부모에 섞여있지 않으면
		if (parentIndex == -1) {
			Long index = parent->Search((*pickGroupStructure)->GetAt(i));
			parent->Delete(index);
			groupStructure->SetParent((*pickGroupStructure)->GetAt(i));

			while (index < dynamic_cast<Block *>(parent)->GetLength()) {
				Structure *childStructure2 = dynamic_cast<Block *>(parent)->GetChild(index);
				if (childStructure2->GetX() == x && childStructure2->GetWidth() == width && childStructure2->GetY() > y) {
					childStructure2->Accept(visitor, 0, -height);
				}
				index++;
			}
		}
		i++;
	}
	
	if (visitor != NULL) {
		delete visitor;
	}

	if (visitorCollector != NULL) {
		delete visitorCollector;
	}
}


