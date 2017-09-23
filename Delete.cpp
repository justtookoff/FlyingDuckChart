//Delete.cpp
#include "Delete.h"
#include "SelectedStructure.h"
#include "Structure.h"
#include "CopyStructure.h"
#include "Collector.h"
#include "NSChart.h"
#include "Mover.h"
#include "GroupStructure.h"
#include "Group.h"
#include "GroupSelectedStructure.h"
#include "GroupIndexes.h"


Delete::Delete(){

}

Delete::Delete(const Delete& source){

}

Delete::~Delete(){

}

void Delete::DeleteFunction(SelectedStructure *selectedStructure, NSChart *nsChart){
	Long i = 0;
	Long index;
	Structure *parentStructure;
	Long j = 0;
	Visitor *visitor;
	Long x;
	Long y;
	Long height;
	Long width;
	Structure *structure;
	visitor = new Collector;
	Visitor *visitorMover = new Mover;

	
	nsChart->Arrange();
	selectedStructure->Arrange();

	while (i <  selectedStructure->GetLength()){

		//부모 구하고
		parentStructure = selectedStructure->GetAt(i)->GetParentStructure();
		//몇번째 인지 부모에서 찾고
		index = parentStructure->Search(selectedStructure->GetAt(i));
		x = selectedStructure->GetAt(i)->GetX();
		y = selectedStructure->GetAt(i)->GetY();
		width = selectedStructure->GetAt(i)->GetWidth();
		height = selectedStructure->GetAt(i)->GetHeight();

		if (dynamic_cast<Block *>(selectedStructure->GetAt(i))){

			Long j = 0;
			while (j != dynamic_cast<Block*>(selectedStructure->GetAt(i))->GetLength()){
				dynamic_cast<Block*>(selectedStructure->GetAt(i))->GetChild(j)->Accept(visitor, nsChart,selectedStructure->GetAt(i));
				
			}

		}

		//그 위치 할당해제
		if (parentStructure->GetChild(index) != 0) {
			delete parentStructure->GetChild(index);
			parentStructure->Delete(index);
		}
	
		while (index < dynamic_cast<Block*>(parentStructure)->GetLength()){

			structure = dynamic_cast<Block*>(parentStructure)->GetChild(index);
			
			if (structure->GetX() == x && structure->GetWidth() == width && structure->GetY() > y){
				structure->Accept(visitorMover, 0, -height);
			}
			index++;	
		}

		i++;
	}
	
	//마지막으로 초기화
	selectedStructure->Clear();

	if (visitor != 0){
	
		delete visitor;
	}

	if (visitorMover != 0){
		delete visitorMover;
	}

}

void Delete::DeleteGroupFunction(GroupIndexes *groupIndexes, GroupSelectedStructure *groupSelectedStructure, GroupStructure *groupStructure, NSChart *nsChart) {
	Long q = 0;
	Group *group = new Group;
	
	//정렬한다.
	nsChart->Arrange();

	//groupSelectedStructure에 있는 모든 그룹에 있는 모든 도형들을 새로 배열한 그룹에 모두 모은다.
	while (q < groupSelectedStructure->GetLength()) {
		Long v = 0;
		while (v < groupSelectedStructure->GetAt(q)->GetLength()) {
			group->Add(groupSelectedStructure->GetAt(q)->GetAt(v));
			v++;
		}
		q++;
	}

	//group에 있는 모든 도형들을 역정렬한다.
	group->ReverseArrange();

	Long i = 0;
	Long index;
	Structure *parentStructure;
	Long j = 0;
	Long k = 0;
	Visitor *visitor;
	Long x;
	Long y;
	Long height;
	Long width;
	Structure *structure;
	visitor = new Collector;
	Visitor *visitorMover = new Mover;
	
	while (k < group->GetLength()) {
		//부모 구하고
		parentStructure = group->GetAt(k)->GetParentStructure();
		//몇번째 인지 부모에서 찾고
		index = parentStructure->Search(group->GetAt(k));
		x = group->GetAt(k)->GetX();
		y = group->GetAt(k)->GetY();
		width = group->GetAt(k)->GetWidth();
		height = group->GetAt(k)->GetHeight();

		if (dynamic_cast<Block *>(group->GetAt(k))) {

			j = 0;
			while (j != dynamic_cast<Block*>(group->GetAt(k))->GetLength()) {
				dynamic_cast<Block*>(group->GetAt(k))->GetChild(j)->Accept(visitor, nsChart, group->GetAt(k));
			}
		}

		//그 위치 할당해제
		if (parentStructure->GetChild(index) != 0) {
			delete parentStructure->GetChild(index);
			parentStructure->Delete(index);
		}

		while (index < dynamic_cast<Block*>(parentStructure)->GetLength()) {

			structure = dynamic_cast<Block*>(parentStructure)->GetChild(index);

			if (structure->GetX() == x && structure->GetWidth() == width && structure->GetY() > y) {
				structure->Accept(visitorMover, 0, -height);
			}
			index++;
		}
		k++;
	}

	//GroupStructure에서 해당하는 그룹을 삭제한다.
	i = 0;
	groupIndexes->Clear();
	while (i < groupSelectedStructure->GetLength()) {
		index = groupStructure->Search(groupSelectedStructure->GetAt(i));
		groupIndexes->Add(index);
		i++;
	}

	groupIndexes->Arrange();

	i = 0;
	while (i < groupIndexes->GetLength()) {
		groupStructure->Delete(groupIndexes->GetAt(i));
		i++;
	}
	
	//GroupSelectedStructure을 클리어해준다.
	groupSelectedStructure->Clear();
	
	if (visitor != 0) {

		delete visitor;
	}

	if (visitorMover != 0) {
		delete visitorMover;
	}

	if (group != 0) {
		delete group;
	}

}


Delete& Delete::operator=(const Delete& source){

	return *this;
}