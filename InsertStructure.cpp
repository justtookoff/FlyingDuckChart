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
	parentStructure = markStructure->GetParentStructure(); //������ ��ġ�� ����Ű�� markStructure�� �θ� ã�´�.
	index = parentStructure->Search(markStructure); //markStructure�� �θ𿡼� markStructure�� ���°���� ã�´�.

	if (checkingResult == 4){ //������ ��ġ�� �����̸�
		structure->SetParent(parentStructure); //������ ������ �θ������ش�.
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
	if (checkingResult == 6){ //������ ��ġ�� �����̸�
		structure->SetParent(parentStructure); //������ ������ �θ������ش�.
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

	parentMarkStructure = markStructure->GetParentStructure(); //������ ��ġ�� ����Ű�� markStructure�� �θ� ã�´�.

	//�׷��� x, y, widht, heigt�� ���Ѵ�.
	Long j = 0;
	Long groupX = insertGroup->GetAt(j)->GetX();
	Long groupY = insertGroup->GetAt(j)->GetY();
	Long groupWidth = groupX + insertGroup->GetAt(j)->GetWidth();
	Long groupHeight = groupY + insertGroup->GetAt(j)->GetHeight();

	while (j < insertGroup->GetLength()) {
		//�ּ� x�� ���Ѵ�.
		if (groupX > insertGroup->GetAt(j)->GetX()) {
			groupX = insertGroup->GetAt(j)->GetX();
		}
		//�ּ� y�� ���Ѵ�.
		if (groupY > insertGroup->GetAt(j)->GetY()) {
			groupY = insertGroup->GetAt(j)->GetY();
		}
		//�ִ� �ʺ� ���Ѵ�.
		if (groupWidth < insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth()) {
			groupWidth = insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth();
		}
		//�ִ� ���̸� ���Ѵ�.
		if (groupHeight < insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight()) {
			groupHeight = insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight();
		}
		j++;
	}

	//markStructure�� x,y, width, height�� ���Ѵ�.
	Long markStructureX;
	Long markStructureY;
	Long markStructureWidth;
	Long markStrutureHeight = markStructure->GetHeight();
	index = parentMarkStructure->Search(markStructure); //markStructure�� �θ𿡼� markStructure�� ���°���� ã�´�. 

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
	//�׷쿡 �ִ� ��� ������ Correct �� �ش�.
	while (i < insertGroup->GetLength()) {

		structure = insertGroup->GetAt(i);
		parentStructure = structure->GetParentStructure();
		differenceWidth = (markStructureWidth * structure->GetWidth()) / (groupWidth - groupX);

		//�θ�� �ڽİ��踦 �����Ѵ�.
		if (structure->GetParentStructure() == structure) {
			
			if (checkingResult == 4 || checkingResult == 6) {
				structure->SetParent(parentMarkStructure); //������ ������ �θ������ش�.
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
				//True �ڸ��̸�
				if (structure->GetX() == parentX) {
					differenceWidth = dynamic_cast<Selection *>(parentStructure)->GetMidX() - parentStructure->GetX();

					if (dynamic_cast<Block *>(structure)) {
						dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
					}
					else {
						structure->Correct(0, 0, differenceWidth, structure->GetHeight());
					}

				}
				//False �ڸ��̸�
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

				//FALSE �ڸ��̸�,
				if (structure->GetX() == midX) {
					if (dynamic_cast<Block *>(structure)) {
						dynamic_cast<Block *>(structure)->CorrectWidth(differenceWidth);
					}
					else {
						structure->Correct(0, 0, differenceWidth, structure->GetHeight());
					}
				}
				//TRUE �ڸ��̸�,
				else if (structure->GetX() < midX) {
					Long j = 0;

					//�� ��° �ڸ����� ���Ѵ�.
					while (parentX + (j * divideWidth) != structure->GetX()) {
						j++;
					}

					//j�ڸ��� ���� Correct �� �ش�.
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

	//���������� �����Ѵ�.
	insertGroup->Arrange();

	//�׷��� ��ü�� �ּ� x, �ּ� y, �ִ� �ʺ�, �ִ� ���̸� ���Ѵ�.
	Long j = 0;
	Long groupX = insertGroup->GetAt(j)->GetX();
	Long groupY = insertGroup->GetAt(j)->GetY();
	Long groupWidth = groupX + insertGroup->GetAt(j)->GetWidth();
	Long groupHeight = groupY + insertGroup->GetAt(j)->GetHeight();

	while (j < insertGroup->GetLength()){
		//�ּ� x�� ���Ѵ�.
		if (groupX > insertGroup->GetAt(j)->GetX()){
			groupX = insertGroup->GetAt(j)->GetX();
		}
		//�ּ� y�� ���Ѵ�.
		if (groupY > insertGroup->GetAt(j)->GetY()){
			groupY = insertGroup->GetAt(j)->GetY();
		}
		//�ִ� �ʺ� ���Ѵ�.
		if (groupWidth < insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth()){
			groupWidth = insertGroup->GetAt(j)->GetX() + insertGroup->GetAt(j)->GetWidth();
		}
		//�ִ� ���̸� ���Ѵ�.
		if (groupHeight < insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight()){
			groupHeight = insertGroup->GetAt(j)->GetY() + insertGroup->GetAt(j)->GetHeight();
		}
		j++;
	}

	//������ ��ġ�� x, y, width, height�� �����Ѵ�.
	Long markStructureX = markStructure->GetX();
	Long markStructureY = markStructure->GetY();
	Long markStructureWidth = markStructure->GetWidth();
	Long markStructureHeight = markStructure->GetHeight();
	
	//������ markStructure�� �θ� ��ȣ�� ã�f��.
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
			//nsChart�� �پ��־��� �θ��
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

