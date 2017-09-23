//Finder.cpp
#include "Finder.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "SelectedStructure.h"
#include "GroupStructure.h"
#include "GroupSelectedStructure.h"
#include "Group.h"

Finder::Finder() {

}

Finder::Finder(const Finder& source) {

}

Finder& Finder::operator=(const Finder& source) {
	return *this;
}

Finder::~Finder() {

}

void Finder::VisitSequence(Sequence *sequence, Structure* *indexStructure, Long x, Long y) {
	if (sequence->GetX() <= x && sequence->GetX() + sequence->GetWidth() >= x && sequence->GetY() <= y && sequence->GetY() + sequence->GetHeight() >= y) {
		*indexStructure = sequence;
	}

}

void Finder::VisitSelection(Selection *selection, Structure* *indexStructure, Long x, Long y) {
	if (selection->GetX() <= x && selection->GetX() + selection->GetWidth() >= x && selection->GetY() <= y && selection->GetY() + selection->GetHeight() >= y) {
		*indexStructure = selection;
	}
		
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->Accept(visitor, indexStructure, x, y);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}

}

void Finder::VisitIteration(Iteration *iteration, Structure* *indexStructure, Long x, Long y) {
	if (iteration->GetX() <= x && iteration->GetX() + iteration->GetWidth() >= x && iteration->GetY() <= y && iteration->GetY() + iteration->GetHeight() >= y) {
		*indexStructure = iteration;
	}
	
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->Accept(visitor, indexStructure, x, y);
		i++;
	}
	if (visitor != 0) {
		delete visitor;
	}
}

void Finder::VisitCase(Case *sCase, Structure* *indexStructure, Long x, Long y) {
	if (sCase->GetX() <= x && sCase->GetX() + sCase->GetWidth() >= x && sCase->GetY() <= y && sCase->GetY() + sCase->GetHeight() >= y) {
		*indexStructure = sCase;
	}
	
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->Accept(visitor, indexStructure, x, y);
		i++;
	}
	if (visitor != 0) {
		delete visitor;
	}
}

void Finder::VisitSequence(Sequence *sequence, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	Long index;
	Long groupIndex;
	Long flag = 0;
	Long i = 0;
	
	if (previousX <= sequence->GetX() && sequence->GetX() <= currentX && previousY <= sequence->GetY() && sequence->GetY() <= currentY &&
		previousX <= sequence->GetX() + sequence->GetWidth() && sequence->GetX() + sequence->GetWidth() <= currentX &&
		previousY <= sequence->GetY() + sequence->GetHeight() && sequence->GetY() + sequence->GetHeight() <= currentY){

		//그룹에 있는 도형인지 찾는다.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(sequence);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//도형에 그룹에 있으면,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//도형이 속한 그룹의 x,y,width,height를 구한다.
			while (j < group2.GetLength()) {
				//최소 x를 구한다.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//최소 y를 구한다.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//최대 너비를 구한다.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//최대 높이를 구한다.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//도형이 속한 그룹이 드래그 범위 안에 있으면,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//그룹이 groupSelectedStructure에 이미 있는지 확인한다.
				index = groupSelectedStructure->Search(&group2);
				//없으면,
				if (index == -1) {
					//그룹을 groupSelectedStructure에 추가한다.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//도형이 그룹에 없으면
		else {
			//도형을 selectedStructure에 추가한다.
			selectedStructure->Add(sequence);
		}
	}
}

void Finder::VisitSelection(Selection *selection, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	Long index;
	Long groupIndex;
	Long flag = 0;
	Long i = 0;
	
	if (previousX <= selection->GetX() && selection->GetX() <= currentX && previousY <= selection->GetY() && selection->GetY() <= currentY &&
		previousX <= selection->GetX() + selection->GetWidth() && selection->GetX() + selection->GetWidth() <= currentX &&
		previousY <= selection->GetY() + selection->GetHeight() && selection->GetY() + selection->GetHeight() <= currentY){

		//그룹에 있는 도형인지 찾는다.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(selection);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//도형에 그룹에 있으면,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//도형이 속한 그룹의 x,y,width,height를 구한다.
			while (j < group2.GetLength()) {
				//최소 x를 구한다.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//최소 y를 구한다.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//최대 너비를 구한다.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//최대 높이를 구한다.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//도형이 속한 그룹이 드래그 범위 안에 있으면,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//그룹이 groupSelectedStructure에 이미 있는지 확인한다.
				index = groupSelectedStructure->Search(&group2);
				//없으면,
				if (index == -1) {
					//그룹을 groupSelectedStructure에 추가한다.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//도형이 그룹에 없으면
		else {
			//도형을 selectedStructure에 추가한다.
			selectedStructure->Add(selection);
		}
	}
	
	i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->Accept(visitor, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
		i++;
	}
	if (visitor != 0) {
		delete visitor;
	}
}

void Finder::VisitIteration(Iteration *iteration, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	Long index;
	Long groupIndex;
	Long flag = 0;
	Long i = 0;
	
	if (previousX <= iteration->GetX() && iteration->GetX() <= currentX && previousY <= iteration->GetY() && iteration->GetY() <= currentY &&
		previousX <= iteration->GetX() + iteration->GetWidth() && iteration->GetX() + iteration->GetWidth() <= currentX &&
		previousY <= iteration->GetY() + iteration->GetHeight() && iteration->GetY() + iteration->GetHeight() <= currentY){

		//그룹에 있는 도형인지 찾는다.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(iteration);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//도형에 그룹에 있으면,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//도형이 속한 그룹의 x,y,width,height를 구한다.
			while (j < group2.GetLength()) {
				//최소 x를 구한다.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//최소 y를 구한다.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//최대 너비를 구한다.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//최대 높이를 구한다.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//도형이 속한 그룹이 드래그 범위 안에 있으면,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//그룹이 groupSelectedStructure에 이미 있는지 확인한다.
				index = groupSelectedStructure->Search(&group2);
				//없으면,
				if (index == -1) {
					//그룹을 groupSelectedStructure에 추가한다.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//도형이 그룹에 없으면
		else {
			//도형을 selectedStructure에 추가한다.
			selectedStructure->Add(iteration);
		}
	}
	
	i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->Accept(visitor, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
		i++;
	}
	if (visitor != 0) {
		delete visitor;
	}
}

void Finder::VisitCase(Case *sCase, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	Long index;
	Long groupIndex;
	Long flag = 0;
	Long i = 0;
	
	if (previousX <= sCase->GetX() && sCase->GetX() <= currentX && previousY <= sCase->GetY() && sCase->GetY() <= currentY &&
		previousX <= sCase->GetX() + sCase->GetWidth() && sCase->GetX() + sCase->GetWidth() <= currentX &&
		previousY <= sCase->GetY() + sCase->GetHeight() && sCase->GetY() + sCase->GetHeight() <= currentY){

		//그룹에 있는 도형인지 찾는다.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(sCase);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//도형에 그룹에 있으면,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//도형이 속한 그룹의 x,y,width,height를 구한다.
			while (j < group2.GetLength()) {
				//최소 x를 구한다.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//최소 y를 구한다.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//최대 너비를 구한다.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//최대 높이를 구한다.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//도형이 속한 그룹이 드래그 범위 안에 있으면,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//그룹이 groupSelectedStructure에 이미 있는지 확인한다.
				index = groupSelectedStructure->Search(&group2);
				//없으면,
				if (index == -1) {
					//그룹을 groupSelectedStructure에 추가한다.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//도형이 그룹에 없으면
		else {
			//도형을 selectedStructure에 추가한다.
			selectedStructure->Add(sCase);
		}
	}
	
	i = 0;
	Structure *structure = 0;
	Visitor *visitor;
	visitor = new Finder;
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->Accept(visitor, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
		i++;
	}
	if (visitor != 0) {
		delete visitor;
	}
}
