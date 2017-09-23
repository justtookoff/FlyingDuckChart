//InsertStructure.cpp
#include "InsertStructure.h"
#include "Structure.h"
#include "Block.h"
#include "Mover.h"
#include "Case.h"
#include "Iteration.h"
#include "Selection.h"
#include "Sequence.h"
#include "Group.h"

InsertStructure::InsertStructure(){

}

InsertStructure::InsertStructure(const InsertStructure& source){

}

InsertStructure::~InsertStructure(){

}

InsertStructure& InsertStructure::operator=(const InsertStructure& source){

	return *this;
}

void InsertStructure::Insert(Structure *insertStructure, Structure *markStructure, Long checkingResult){
	Structure *parentStructure;
	Structure *structure;
	Structure *childStructure;
	Long x;
	Long width;
	Long index;
	Visitor *visitor = new Mover;

	structure = insertStructure;
	parentStructure = markStructure->GetParentStructure(); //삽입할 위치를 가르키는 markStructure의 부모를 찾는다.
	index = parentStructure->Search(markStructure); //markStructure의 부모에서 markStructure가 몇번째인지 찾는다.

	if (checkingResult == 4){ //삽입할 위치가 윗변이면
		structure->SetParent(parentStructure); //삽입할 구조를 부모설정해준다.
		dynamic_cast<Block*>(parentStructure)->Add(structure);

		dynamic_cast<Structure*>(structure)->Correct(markStructure->GetX() - structure->GetX(), 
			-structure->GetY() + markStructure->GetY(), markStructure->GetWidth(), structure->GetHeight());
		x = structure->GetX();
		width = structure->GetWidth();

		while (index < dynamic_cast<Block*>(parentStructure)->GetLength() - 1){
			childStructure = dynamic_cast<Block*>(parentStructure)->GetChild(index);
			if (childStructure->GetX() == x && childStructure->GetWidth() == width){
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}
	if (checkingResult == 6){ //삽입할 위치가 윗변이면
		structure->SetParent(parentStructure); //삽입할 구조를 부모설정해준다.
		dynamic_cast<Block*>(parentStructure)->Add(structure);

		dynamic_cast<Structure*>(structure)->Correct(markStructure->GetX() - structure->GetX(), 
			-structure->GetY() + markStructure->GetY() + markStructure->GetHeight(), markStructure->GetWidth(), structure->GetHeight());
		x = structure->GetX();
		width = structure->GetWidth();
		index++;
		while (index < dynamic_cast<Block*>(parentStructure)->GetLength() - 1){
			childStructure = dynamic_cast<Block*>(parentStructure)->GetChild(index);
			if (childStructure->GetX() == x && childStructure->GetWidth() == width){
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}
	if (checkingResult == 8){
		structure->SetParent(markStructure);
		dynamic_cast<Block*>(markStructure)->Add(structure);

		dynamic_cast<Structure*>(structure)->Correct(dynamic_cast<Block*>(markStructure)->GetMidX() - structure->GetX(), 
			dynamic_cast<Block*>(markStructure)->GetMidY() - structure->GetY(), 
			dynamic_cast<Block*>(markStructure)->GetX() + dynamic_cast<Block*>(markStructure)->GetWidth() - dynamic_cast<Block*>(markStructure)->GetMidX(), 
			structure->GetHeight());
		index = 0;
		while (index < dynamic_cast<Block*>(markStructure)->GetLength() - 1){
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			childStructure->Accept(visitor, 0, structure->GetHeight());
			index++;
		}
	}

	if (checkingResult == 9){
		structure->SetParent(markStructure);
		dynamic_cast<Block*>(markStructure)->Add(structure);

		dynamic_cast<Structure*>(structure)->Correct(markStructure->GetX() - structure->GetX(), 
			dynamic_cast<Block*>(markStructure)->GetMidY() - structure->GetY(), 
			dynamic_cast<Block*>(markStructure)->GetMidX() - dynamic_cast<Block*>(markStructure)->GetX(), structure->GetHeight());
		index = 0;
		while (index < dynamic_cast<Block*>(markStructure)->GetLength() - 1){
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (structure->GetX() == childStructure->GetX()){
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}
	if (checkingResult == 10){
		structure->SetParent(markStructure);
		dynamic_cast<Block*>(markStructure)->Add(structure);

		dynamic_cast<Structure*>(structure)->Correct(dynamic_cast<Block*>(markStructure)->GetMidX() - structure->GetX(), 
			dynamic_cast<Block*>(markStructure)->GetMidY() - structure->GetY(), 
			dynamic_cast<Block*>(markStructure)->GetX() + dynamic_cast<Block*>(markStructure)->GetWidth() - dynamic_cast<Block*>(markStructure)->GetMidX(), 
			structure->GetHeight());
		index = 0;
		while (index < dynamic_cast<Block*>(markStructure)->GetLength() - 1){
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (dynamic_cast<Block*>(markStructure)->GetMidX() == childStructure->GetX()){
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}

	if (checkingResult == 11) {
		structure->SetParent(markStructure);
		dynamic_cast<Block *>(markStructure)->Add(structure);

		dynamic_cast<Structure *>(structure)->Correct(dynamic_cast<Block *>(markStructure)->GetMidX() - structure->GetX(),
			dynamic_cast<Block *>(markStructure)->GetMidY() - structure->GetY(),
			dynamic_cast<Block *>(markStructure)->GetX() + dynamic_cast<Block *>(markStructure)->GetWidth() - dynamic_cast<Block *>(markStructure)->GetMidX(),
			structure->GetHeight());
		index = 0;
		while (index < dynamic_cast<Block*>(markStructure)->GetLength() - 1) {
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (dynamic_cast<Block*>(markStructure)->GetMidX() == childStructure->GetX()) {
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}

	if (checkingResult >= 12) {
		structure->SetParent(markStructure);
		dynamic_cast<Block *>(markStructure)->Add(structure);

		Long j = checkingResult - 12;
		Long midY2 = dynamic_cast<Block *>(markStructure)->GetMidY() + 30;
		Long count = dynamic_cast<Case *>(markStructure)->GetCount();
		Long standardWidth = dynamic_cast<Block *>(markStructure)->GetMidX() - dynamic_cast<Block *>(markStructure)->GetX();
		Long divideWidth = standardWidth / (count + 1);

		if (checkingResult == 12 + count) {
			dynamic_cast<Structure *>(structure)->Correct((divideWidth *j) + dynamic_cast<Case *>(markStructure)->GetX() - structure->GetX(),
				midY2 - structure->GetY(), dynamic_cast<Block *>(markStructure)->GetMidX() - (markStructure->GetX() + (j *divideWidth)), structure->GetHeight());
		}
		else {
			dynamic_cast<Structure *>(structure)->Correct((divideWidth *j) + dynamic_cast<Case *>(markStructure)->GetX() - structure->GetX(),
				midY2 - structure->GetY(), divideWidth, structure->GetHeight());
		}
		index = 0;
		while (index < dynamic_cast<Block *>(markStructure)->GetLength() - 1) {
			childStructure = dynamic_cast<Block *>(markStructure)->GetChild(index);
			if ((divideWidth * j) + dynamic_cast<Case *>(markStructure)->GetX() == childStructure->GetX()) {
				childStructure->Accept(visitor, 0, structure->GetHeight());
			}
			index++;
		}
	}

	if (visitor != NULL){
		delete visitor;
	}
}

void InsertStructure::InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult) {
	Structure *parentMarkStructure;
	Structure *structure;
	Structure *childStructure;
	Structure *parentStructure;
	Long index = 0;
	Visitor *visitor = new Mover;
	Long i = 0;
	Long differenceWidth = 0;

	parentMarkStructure = markStructure->GetParentStructure(); //삽입할 위치를 가르키는 markStructure의 부모를 찾는다.

	//그룹의 x, y, widht, heigt를 구한다.
	Long j = 0;
	Long groupX = insertGroup->GetAt(j)->GetX();
	Long groupY = insertGroup->GetAt(j)->GetY();
	Long groupWidth = groupX + insertGroup->GetAt(j)->GetWidth();
	Long groupHeight = groupY + insertGroup->GetAt(j)->GetHeight();

	while (j < insertGroup->GetLength()) {
		//최소 x를 구한다.
		if (groupX > insertGroup->GetAt(j)->GetX()) {
			groupX = insertGroup->GetAt(j)->GetX();
		}
		//최소 y를 구한다.
		if (groupY > insertGroup->GetAt(j)->GetY()) {
			groupY = insertGroup->GetAt(j)->GetY();
		}
		//최대 너비를 구한다.
		if (groupWidth < insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth()) {
			groupWidth = insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth();
		}
		//최대 높이를 구한다.
		if (groupHeight < insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight()) {
			groupHeight = insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight();
		}
		j++;
	}

	//markStructure의 x,y, width, height를 구한다.
	Long markStructureX;
	Long markStructureY;
	Long markStructureWidth;
	Long markStrutureHeight = markStructure->GetHeight();
	index = parentMarkStructure->Search(markStructure); //markStructure의 부모에서 markStructure가 몇번째인지 찾는다. 

	if (checkingResult == 4) {
		markStructureX = markStructure->GetX();
		markStructureY = markStructure->GetY();
		markStructureWidth = markStructure->GetWidth();

		while (index < dynamic_cast<Block*>(parentMarkStructure)->GetLength()) {
			childStructure = dynamic_cast<Block*>(parentMarkStructure)->GetChild(index);
			if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
				childStructure->Accept(visitor, 0, groupHeight - groupY);
			}
			index++;
		}
	}

	else if (checkingResult == 6) {
		markStructureX = markStructure->GetX();
		markStructureY = markStructure->GetY() + markStrutureHeight;
		markStructureWidth = markStructure->GetWidth();
		index++;

		while (index < dynamic_cast<Block*>(parentMarkStructure)->GetLength()) {
			childStructure = dynamic_cast<Block*>(parentMarkStructure)->GetChild(index);
			if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
				childStructure->Accept(visitor, 0, groupHeight - groupY);
			}
			index++;
		}
	}

	else if (checkingResult == 9) {
		markStructureX = markStructure->GetX();
		markStructureY = dynamic_cast<Block *>(markStructure)->GetMidY();
		markStructureWidth = dynamic_cast<Block *>(markStructure)->GetMidX() - markStructureX;
		index = 0;

		while (index < dynamic_cast<Block*>(markStructure)->GetLength()) {
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
				childStructure->Accept(visitor, 0, groupHeight - groupY);
			}
			index++;
		}
	}

	else if (checkingResult == 8 || checkingResult == 10 || checkingResult == 11) {
		markStructureX = dynamic_cast<Block *>(markStructure)->GetMidX();
		markStructureY = dynamic_cast<Block *>(markStructure)->GetMidY();
		markStructureWidth = markStructure->GetX() + markStructure->GetWidth() - markStructureX;
		index = 0;
		while (index < dynamic_cast<Block*>(markStructure)->GetLength()) {
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
				childStructure->Accept(visitor, 0, groupHeight - groupY);
			}
			index++;
		}
	}
	else if (checkingResult >= 12) {
		Long j = checkingResult - 12;
		Long midY2 = dynamic_cast<Block *>(markStructure)->GetMidY() + 30;
		Long count = dynamic_cast<Case *>(markStructure)->GetCount();
		Long standardWidth = dynamic_cast<Block *>(markStructure)->GetMidX() - dynamic_cast<Block *>(markStructure)->GetX();
		Long divideWidth = standardWidth / (count + 1);
		index = 0;
		markStructureX = (divideWidth *j) + markStructure->GetX();
		markStructureY = midY2;

		if (checkingResult == 12 + count) {
			markStructureWidth = dynamic_cast<Block *>(markStructure)->GetMidX() - markStructureX;
		}
		else {
			markStructureWidth = divideWidth;
		}

		while (index < dynamic_cast<Block*>(markStructure)->GetLength()) {
			childStructure = dynamic_cast<Block*>(markStructure)->GetChild(index);
			if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
				childStructure->Accept(visitor, 0, groupHeight - groupY);
			}
			index++;
		}
	}


	insertGroup->Arrange();
	//그룹에 있는 모든 도형을 Correct 해 준다.
	while (i < insertGroup->GetLength()) {

		structure = insertGroup->GetAt(i);
		parentStructure = structure->GetParentStructure();
		differenceWidth = (markStructureWidth * structure->GetWidth()) / (groupWidth - groupX);

		//부모와 자식관계를 설정한다.
		if (structure->GetParentStructure() == structure) {
			
			if (checkingResult == 4 || checkingResult == 6) {
				structure->SetParent(parentMarkStructure); //삽입할 구조를 부모설정해준다.
				parentMarkStructure->Add(structure);
			}
			else {
				structure->SetParent(markStructure);
				markStructure->Add(structure);
			}

			if (dynamic_cast<Block *>(structure)) {
				dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
			}
			else {
				structure->Correct(0, 0, differenceWidth, structure->GetHeight());
			}

			structure->Accept(visitor, markStructureX - groupX, markStructureY - groupY);

			
		}

		else {
			if (dynamic_cast<Iteration *>(parentStructure)) {

				differenceWidth = parentStructure->GetX() + parentStructure->GetWidth() - dynamic_cast<Iteration *>(parentStructure)->GetMidX();

				if (dynamic_cast<Block *>(structure)) {
					dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
				}
				else {
					structure->Correct(0, 0, differenceWidth, structure->GetHeight());
				}


			}

			else if (dynamic_cast<Selection *>(parentStructure)) {
				Long parentX = parentStructure->GetX();
				Long parentMidX = dynamic_cast<Selection *>(parentStructure)->GetMidX();
				//True 자리이면
				if (structure->GetX() == parentX) {
					differenceWidth = dynamic_cast<Selection *>(parentStructure)->GetMidX() - parentStructure->GetX();

					if (dynamic_cast<Block *>(structure)) {
						dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
					}
					else {
						structure->Correct(0, 0, differenceWidth, structure->GetHeight());
					}

				}
				//False 자리이면
				else if (structure->GetX() == parentMidX) {
					differenceWidth = parentStructure->GetX() + parentStructure->GetWidth() - dynamic_cast<Selection *>(parentStructure)->GetMidX();

					if (dynamic_cast<Block *>(structure)) {
						dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
					}
					else {
						structure->Correct(0, 0, differenceWidth, structure->GetHeight());
					}

				}

			}
			else if (dynamic_cast<Case *>(parentStructure)) {
				Long parentX = parentStructure->GetX();
				Long parentY = parentStructure->GetY();
				Long parentWidth = parentStructure->GetWidth();
				Long midX = dynamic_cast<Case *>(parentStructure)->GetMidX();
				Long midY = dynamic_cast<Case *>(parentStructure)->GetMidY();
				Long count = dynamic_cast<Case *>(parentStructure)->GetCount();
				Long standardWidth = midX - parentStructure->GetX();
				Long divideWidth = standardWidth / (count + 1);
				differenceWidth = parentX + parentWidth - midX;

				//FALSE 자리이면,
				if (structure->GetX() == midX) {
					if (dynamic_cast<Block *>(structure)) {
						dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
					}
					else {
						structure->Correct(0, 0, differenceWidth, structure->GetHeight());
					}
				}
				//TRUE 자리이면,
				else if (structure->GetX() < midX) {
					Long j = 0;

					//몇 번째 자리인지 구한다.
					while (parentX + (j * divideWidth) != structure->GetX()) {
						j++;
					}

					//j자리에 따라 Correct 해 준다.
					if (j == count) {
						divideWidth = midX - (parentX + (j * divideWidth));
						if (dynamic_cast<Block *>(structure)) {
							dynamic_cast<Block *>(structure)->CorrectWidth(divideWidth);
						}
						else {
							structure->Correct(0, 0, divideWidth, structure->GetHeight());
						}
					}
					else {
						if (dynamic_cast<Block *>(structure)) {
							dynamic_cast<Block *>(structure)->CorrectWidth(divideWidth);
						}
						else {
							structure->Correct(0, 0, divideWidth, structure->GetHeight());
						}
					}
				}
			}
		}
		i++;
	}

	if (visitor != NULL) {
		delete visitor;
	}
}

/*
void InsertStructure::InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult) {
	Structure *parentStructure;
	Structure *structure;
	Structure *childStructure;
	Long x;
	Long width;
	Long index;
	Visitor *visitor = new Mover;

	//순방향으로 정렬한다.
	insertGroup->Arrange();

	//그룹의 전체의 최소 x, 최소 y, 최대 너비, 최대 높이를 구한다.
	Long j = 0;
	Long groupX = insertGroup->GetAt(j)->GetX();
	Long groupY = insertGroup->GetAt(j)->GetY();
	Long groupWidth = groupX + insertGroup->GetAt(j)->GetWidth();
	Long groupHeight = groupY + insertGroup->GetAt(j)->GetHeight();

	while (j < insertGroup->GetLength()){
		//최소 x를 구한다.
		if (groupX > insertGroup->GetAt(j)->GetX()){
			groupX = insertGroup->GetAt(j)->GetX();
		}
		//최소 y를 구한다.
		if (groupY > insertGroup->GetAt(j)->GetY()){
			groupY = insertGroup->GetAt(j)->GetY();
		}
		//최대 너비를 구한다.
		if (groupWidth < insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth()){
			groupWidth = insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth();
		}
		//최대 높이를 구한다.
		if (groupHeight < insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight()){
			groupHeight = insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight();
		}
		j++;
	}

	//삽입할 위치의 x, y, width, height을 저장한다.
	Long markStructureX = markStructure->GetX();
	Long markStructureY = markStructure->GetY();
	Long markStructureWidth = markStructure->GetWidth();
	Long markStructureHeight = markStructure->GetHeight();
	
	//삽입할 markStructure의 부모 기호를 찾늗다.
	parentStructure = markStructure->GetParentStructure();

	index = parentStructure->Search(markStructure);

	while (index < dynamic_cast<Block*>(parentStructure)->GetLength()) {
		childStructure = dynamic_cast<Block*>(parentStructure)->GetChild(index);
		if (childStructure->GetX() == markStructureX && childStructure->GetWidth() == markStructureWidth) {
			childStructure->Accept(visitor, 0, groupHeight);
		}
		index++;
	}

	Long i = 0;

	while (i < insertGroup->GetLength()) {

		structure = insertGroup->GetAt(i);
		
		Long ratioWidth = (markStructureWidth * structure->GetWidth()) / groupWidth;

		if (checkingResult == 4) {
			//nsChart에 붙어있었던 부모들
			if (structure == structure->GetParentStructure()){
				structure->SetParent(parentStructure);
				dynamic_cast<Block*>(parentStructure)->Add(structure);

				dynamic_cast<Structure*>(structure)->Correct(markStructureX - groupX,
					markStructureY - groupY, ratioWidth, structure->GetHeight());

			}
			else{
				if (dynamic_cast<Iteration*>(structure->GetParentStructure())){
					dynamic_cast<Structure*>(structure)->Correct(dynamic_cast<Iteration*>(structure->GetParentStructure())->GetMidX() - structure->GetX(),
						dynamic_cast<Iteration*>(structure->GetParentStructure())->GetMidY() - structure->GetY() + markStructureY - groupY, 
						ratioWidth, structure->GetHeight());
				}
			}
		
		}
		i++;
	}
	if (visitor != NULL) {
		delete visitor;
	}
}
*/

