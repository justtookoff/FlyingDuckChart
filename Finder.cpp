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

		//�׷쿡 �ִ� �������� ã�´�.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(sequence);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//������ �׷쿡 ������,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//������ ���� �׷��� x,y,width,height�� ���Ѵ�.
			while (j < group2.GetLength()) {
				//�ּ� x�� ���Ѵ�.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//�ּ� y�� ���Ѵ�.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//�ִ� �ʺ� ���Ѵ�.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//�ִ� ���̸� ���Ѵ�.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//������ ���� �׷��� �巡�� ���� �ȿ� ������,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//�׷��� groupSelectedStructure�� �̹� �ִ��� Ȯ���Ѵ�.
				index = groupSelectedStructure->Search(&group2);
				//������,
				if (index == -1) {
					//�׷��� groupSelectedStructure�� �߰��Ѵ�.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//������ �׷쿡 ������
		else {
			//������ selectedStructure�� �߰��Ѵ�.
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

		//�׷쿡 �ִ� �������� ã�´�.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(selection);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//������ �׷쿡 ������,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//������ ���� �׷��� x,y,width,height�� ���Ѵ�.
			while (j < group2.GetLength()) {
				//�ּ� x�� ���Ѵ�.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//�ּ� y�� ���Ѵ�.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//�ִ� �ʺ� ���Ѵ�.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//�ִ� ���̸� ���Ѵ�.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//������ ���� �׷��� �巡�� ���� �ȿ� ������,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//�׷��� groupSelectedStructure�� �̹� �ִ��� Ȯ���Ѵ�.
				index = groupSelectedStructure->Search(&group2);
				//������,
				if (index == -1) {
					//�׷��� groupSelectedStructure�� �߰��Ѵ�.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//������ �׷쿡 ������
		else {
			//������ selectedStructure�� �߰��Ѵ�.
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

		//�׷쿡 �ִ� �������� ã�´�.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(iteration);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//������ �׷쿡 ������,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//������ ���� �׷��� x,y,width,height�� ���Ѵ�.
			while (j < group2.GetLength()) {
				//�ּ� x�� ���Ѵ�.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//�ּ� y�� ���Ѵ�.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//�ִ� �ʺ� ���Ѵ�.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//�ִ� ���̸� ���Ѵ�.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//������ ���� �׷��� �巡�� ���� �ȿ� ������,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//�׷��� groupSelectedStructure�� �̹� �ִ��� Ȯ���Ѵ�.
				index = groupSelectedStructure->Search(&group2);
				//������,
				if (index == -1) {
					//�׷��� groupSelectedStructure�� �߰��Ѵ�.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//������ �׷쿡 ������
		else {
			//������ selectedStructure�� �߰��Ѵ�.
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

		//�׷쿡 �ִ� �������� ã�´�.
		while (i < groupStructure->GetLength()) {
			Group group = groupStructure->GetAt(i);
			index = group.Search(sCase);
			if (index != -1) {
				flag++;
				groupIndex = i;
			}
			i++;
		}

		//������ �׷쿡 ������,
		if (flag != 0) {
			Long j = 0;
			Group group2 = groupStructure->GetAt(groupIndex);
			Long x = group2.GetAt(j)->GetX();
			Long y = group2.GetAt(j)->GetY();
			Long width = x + group2.GetAt(j)->GetWidth();
			Long height = y + group2.GetAt(j)->GetHeight();

			//������ ���� �׷��� x,y,width,height�� ���Ѵ�.
			while (j < group2.GetLength()) {
				//�ּ� x�� ���Ѵ�.
				if (x > group2.GetAt(j)->GetX()) {
					x = group2.GetAt(j)->GetX();
				}
				//�ּ� y�� ���Ѵ�.
				if (y > group2.GetAt(j)->GetY()) {
					y = group2.GetAt(j)->GetY();
				}
				//�ִ� �ʺ� ���Ѵ�.
				if (width < group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth()) {
					width = group2.GetAt(j)->GetX() + group2.GetAt(j)->GetWidth();
				}
				//�ִ� ���̸� ���Ѵ�.
				if (height < group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight()) {
					height = group2.GetAt(j)->GetY() + group2.GetAt(j)->GetHeight();
				}
				j++;
			}

			//������ ���� �׷��� �巡�� ���� �ȿ� ������,
			if (previousX <= x  &&  x <= currentX && previousX <= width && width <= currentX && previousY <= y && y <= currentY && previousY <= height && height <= currentY) {
				//�׷��� groupSelectedStructure�� �̹� �ִ��� Ȯ���Ѵ�.
				index = groupSelectedStructure->Search(&group2);
				//������,
				if (index == -1) {
					//�׷��� groupSelectedStructure�� �߰��Ѵ�.
					groupSelectedStructure->Add(&groupStructure->GetAt(groupIndex));
				}

			}

		}
		//������ �׷쿡 ������
		else {
			//������ selectedStructure�� �߰��Ѵ�.
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
