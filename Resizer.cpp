//Resizer.cpp

#include "Resizer.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "Visitor.h"

Resizer::Resizer() {

}

Resizer::Resizer(const Resizer& source) {

}

Resizer::~Resizer() {

}

Resizer& Resizer::operator=(const Resizer& source) {
	return *this;
}

void Resizer::VisitSequence1(Sequence *sequence, Long x, Long midX, Long previousX) {
	Structure *parent = sequence->GetParentStructure();
	//���ϱ��� x���� ���Ѵ�.
	Long parentX = parent->GetX();
	
	//�θ� ���ù��̸�,
	if (dynamic_cast<Selection *>(parent)) {
		
		//FALSE �ڸ��̸�,
		if (sequence->GetX() == midX) {
			sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
		}
		//TRUE �ڸ��̸�,
		else {
			sequence->Correct(parent->GetX() - sequence->GetX(), 0, dynamic_cast<Block *>(parent)->GetMidX() - parentX, sequence->GetHeight());
		}
	}

	//�θ� �ݺ����̸�,
	else if(dynamic_cast<Iteration *>(parent)){
	    
		sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
	}

	//�θ� ���̽��̸�,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth =  midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);
		
		//FALSE �ڸ��̸�,
		if (sequence->GetX() == midX) {
			sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
		}
		//TRUE �ڸ��̸�,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//�� ��° �ڸ����� ���Ѵ�.
			while (previousX + (j * previousDivideWidth) != sequence->GetX()) {
				j++;
			}

			//ù��° �ڸ��̸�,
			if (j == 0) {
				sequence->Correct(parentX - sequence->GetX(), 0, divideWidth, sequence->GetHeight());
			}
			else {

				//default �ڸ��̸�,
				if (j == count) {
					sequence->Correct(parentX + (j *divideWidth) - sequence->GetX(), 0, midX2 - (parentX + (j *divideWidth)), sequence->GetHeight());
				}
				else {
					sequence->Correct(parentX + (j *divideWidth) - sequence->GetX(), 0, divideWidth, sequence->GetHeight());
				}
			}
		}
	}
}

void Resizer::VisitSelection1(Selection *selection, Long x, Long midX, Long previousX) {
	Structure *parent = selection->GetParentStructure();
	Long parentX = parent->GetX();
	//���ϱ��� x���� ���Ѵ�.
	Long currentX = selection->GetX();
	
	Visitor *visitor = new Resizer;
	//���ϱ��� midX���� ���Ѵ�.
	Long midX2 = selection->GetMidX();
	

	//�θ� ���ù��̸�,
	if (dynamic_cast<Selection *>(parent)) {
		
		//FALSE �ڸ��̸�,
		if (selection->GetX() == midX) {
			selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Selection *>(parent)->GetMidX(), x);
		}
		//TRUE �ڸ��̸�,
		else {
			selection->CorrectXWidthMidX(parent->GetX() - selection->GetX(), dynamic_cast<Selection *>(parent)->GetMidX() - parent->GetX(), x);
		}
	}

	//�θ� �ݺ����̸�,
	else if(dynamic_cast<Iteration *>(parent)){
		
		selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
	}

	//�θ� ���̽��̸�,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE �ڸ��̸�,
		if (selection->GetX() == midX) {
			selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE �ڸ��̸�,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//�� ��° �ڸ����� ���Ѵ�.
			while (previousX + (j * previousDivideWidth) != selection->GetX()) {
				j++;
			}

			//ù��° �ڸ��̸�,
			if (j == 0) {
				selection->CorrectXWidthMidX(parentX - selection->GetX(), divideWidth, x);
			}
			else {

				//default �ڸ��̸�,
				if (j == count) {
					selection->CorrectXWidthMidX(parentX + (j *divideWidth) - selection->GetX(), midX2 - (parentX + (j *divideWidth)), x);
				}
				else {
					selection->CorrectXWidthMidX(parentX + (j *divideWidth) - selection->GetX(), divideWidth, x);
				}
			}
		}
	}

	//�ڽĵ� ��� ��ͷ� Resizer�Ѵ�.
	Long i = 0;
	while (i < selection->GetLength()) {
		Structure *structure = selection->GetChild(i);
		structure->Accept1(visitor, x, midX2, currentX);
		i++;
	}

	if (visitor != NULL) {
		delete visitor;
	}

}

void Resizer::VisitIteration1(Iteration *iteration, Long x, Long midX, Long previousX) {
	Structure *parent = iteration->GetParentStructure();
	Long parentX = parent->GetX();
	//���ϱ� �� midX���� ���Ѵ�.
	Long midX2 = iteration->GetMidX();
	//���ϱ� �� x���� ���Ѵ�.
	Long currentX = iteration->GetX();

	//�θ� ���ù��̸�,
	if (dynamic_cast<Selection *>(parent)) {
		//FALSE �ڸ��̸�,
		if (iteration->GetX() == midX) {
			iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Selection *>(parent)->GetMidX());
		}
		//TRUE �ڸ��̸�,
		else {
			iteration->CorrectXWidth(parent->GetX() - iteration->GetX(), dynamic_cast<Selection *>(parent)->GetMidX() - parent->GetX());
		}
	}
	//�θ� �ݺ����̸�,
	else if(dynamic_cast<Iteration *>(parent)){
		
		iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX());
	}

	//�θ� ���̽��̸�,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE �ڸ��̸�,
		if (iteration->GetX() == midX) {
			iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX());
		}
		//TRUE �ڸ��̸�,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//�� ��° �ڸ����� ���Ѵ�.
			while (previousX + (j * previousDivideWidth) != iteration->GetX()) {
				j++;
			}

			//ù��° �ڸ��̸�,
			if (j == 0) {
				iteration->CorrectXWidth(parentX - iteration->GetX(), divideWidth);
			}
			else {

				//default �ڸ��̸�,
				if (j == count) {
					iteration->CorrectXWidth(parentX + (j *divideWidth) - iteration->GetX(), midX2 - (parentX + (j *divideWidth)));
				}
				else {
					iteration->CorrectXWidth(parentX + (j *divideWidth) - iteration->GetX(), divideWidth);
				}
			}
		}
	}
	
	Visitor *visitor = new Resizer;
	
	//�ڽĵ� ��� ��ͷ� Resizer�Ѵ�.
	Long i = 0;
	while (i < iteration->GetLength()) {
		Structure *structure = iteration->GetChild(i);
		structure->Accept1(visitor, x, midX2, currentX);
		i++;
	}

	if (visitor != NULL) {
		delete visitor;
	}
}

void Resizer::VisitCase1(Case *sCase, Long x, Long midX, Long previousX) {
	Structure *parent = sCase->GetParentStructure();
	Long parentX = parent->GetX();
	//���ϱ� �� midX���� ���Ѵ�.
	Long midX2 = dynamic_cast<Case *>(sCase)->GetMidX();
	//���ϱ� �� x���� ���Ѵ�.
	Long currentX = sCase->GetX();
	
	//�θ� ���ù��̸�,
	if (dynamic_cast<Selection *>(parent)) {
		//FALSE �ڸ��̸�,
		if (sCase->GetX() == midX) {
			sCase->CorrectXWidthMidX(dynamic_cast<Selection *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE �ڸ��̸�,
		else {
			sCase->CorrectXWidthMidX(parent->GetX() - sCase->GetX(), dynamic_cast<Block *>(parent)->GetMidX() - parentX, x);
		}
	}
	//�θ� �ݺ����̸�,
	else if (dynamic_cast<Iteration *>(parent)){
		
		sCase->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
	}

	//�θ� ���̽��̸�,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE �ڸ��̸�,
		if (sCase->GetX() == midX) {
			sCase->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE �ڸ��̸�,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//�� ��° �ڸ����� ���Ѵ�.
			while (previousX + (j * previousDivideWidth) != sCase->GetX()) {
				j++;
			}

			//ù��° �ڸ��̸�,
			if (j == 0) {
				sCase->CorrectXWidthMidX(parentX - sCase->GetX(), divideWidth, x);
			}
			else {

				//default �ڸ��̸�,
				if (j == count) {
					sCase->CorrectXWidthMidX(parentX + (j *divideWidth) - sCase->GetX(), midX2 - (parentX + (j *divideWidth)), x);
				}
				else {
					sCase->CorrectXWidthMidX(parentX + (j *divideWidth) - sCase->GetX(), divideWidth, x);
				}
			}
		}
	}

	

	Visitor *visitor = new Resizer;

	//�ڽĵ� ��� ��ͷ� Resizer�Ѵ�.
	Long i = 0;
	while (i < sCase->GetLength()) {
		Structure *structure = sCase->GetChild(i);
		structure->Accept1(visitor, x, midX2, currentX);
		i++;
	}

	if (visitor != NULL) {
		delete visitor;
	}
}

