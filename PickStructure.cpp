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

	//�ڽĵ��� collector�� ������ �̵���Ų��.
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

		//�ڽĵ��� collector�� ������ �̵���Ų��.
		if (dynamic_cast<Block *>(groupStructure)) {
			Long q = 0;
			while (q < dynamic_cast<Block *>((*pickGroupStructure)->GetAt(i))->GetLength()) {
				
				Structure *childStructure = dynamic_cast<Block *>((*pickGroupStructure)->GetAt(i))->GetChild(q);
				//�ڽ��� GroupStructure�� ���� �ִ��� Ȯ���Ѵ�.
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

		//�׷�ȭ�� ��Ʈ���ĵ� �߿� �ϳ��� ��Ʈ������ �θ� ã�´�.
		Structure *parent = (*pickGroupStructure)->GetAt(i)->GetParentStructure();

		//�� �߿� ���°���� ã�´�.
		Long parentIndex = groupSelectedStructure->GetAt(0)->Search(parent);

		//�θ� �������� ������
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


