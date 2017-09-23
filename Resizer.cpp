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
	//변하기전 x값을 구한다.
	Long parentX = parent->GetX();
	
	//부모가 선택문이면,
	if (dynamic_cast<Selection *>(parent)) {
		
		//FALSE 자리이면,
		if (sequence->GetX() == midX) {
			sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
		}
		//TRUE 자리이면,
		else {
			sequence->Correct(parent->GetX() - sequence->GetX(), 0, dynamic_cast<Block *>(parent)->GetMidX() - parentX, sequence->GetHeight());
		}
	}

	//부모가 반복문이면,
	else if(dynamic_cast<Iteration *>(parent)){
	    
		sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
	}

	//부모가 케이스이면,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth =  midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);
		
		//FALSE 자리이면,
		if (sequence->GetX() == midX) {
			sequence->Correct(dynamic_cast<Block *>(parent)->GetMidX() - sequence->GetX(), 0, parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), sequence->GetHeight());
		}
		//TRUE 자리이면,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//몇 번째 자리인지 구한다.
			while (previousX + (j * previousDivideWidth) != sequence->GetX()) {
				j++;
			}

			//첫번째 자리이면,
			if (j == 0) {
				sequence->Correct(parentX - sequence->GetX(), 0, divideWidth, sequence->GetHeight());
			}
			else {

				//default 자리이면,
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
	//변하기전 x값을 구한다.
	Long currentX = selection->GetX();
	
	Visitor *visitor = new Resizer;
	//변하기전 midX값을 구한다.
	Long midX2 = selection->GetMidX();
	

	//부모가 선택문이면,
	if (dynamic_cast<Selection *>(parent)) {
		
		//FALSE 자리이면,
		if (selection->GetX() == midX) {
			selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Selection *>(parent)->GetMidX(), x);
		}
		//TRUE 자리이면,
		else {
			selection->CorrectXWidthMidX(parent->GetX() - selection->GetX(), dynamic_cast<Selection *>(parent)->GetMidX() - parent->GetX(), x);
		}
	}

	//부모가 반복문이면,
	else if(dynamic_cast<Iteration *>(parent)){
		
		selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
	}

	//부모가 케이스이면,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE 자리이면,
		if (selection->GetX() == midX) {
			selection->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - selection->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE 자리이면,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//몇 번째 자리인지 구한다.
			while (previousX + (j * previousDivideWidth) != selection->GetX()) {
				j++;
			}

			//첫번째 자리이면,
			if (j == 0) {
				selection->CorrectXWidthMidX(parentX - selection->GetX(), divideWidth, x);
			}
			else {

				//default 자리이면,
				if (j == count) {
					selection->CorrectXWidthMidX(parentX + (j *divideWidth) - selection->GetX(), midX2 - (parentX + (j *divideWidth)), x);
				}
				else {
					selection->CorrectXWidthMidX(parentX + (j *divideWidth) - selection->GetX(), divideWidth, x);
				}
			}
		}
	}

	//자식들 모두 재귀로 Resizer한다.
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
	//변하기 전 midX값을 구한다.
	Long midX2 = iteration->GetMidX();
	//변하기 전 x값을 구한다.
	Long currentX = iteration->GetX();

	//부모가 선택문이면,
	if (dynamic_cast<Selection *>(parent)) {
		//FALSE 자리이면,
		if (iteration->GetX() == midX) {
			iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Selection *>(parent)->GetMidX());
		}
		//TRUE 자리이면,
		else {
			iteration->CorrectXWidth(parent->GetX() - iteration->GetX(), dynamic_cast<Selection *>(parent)->GetMidX() - parent->GetX());
		}
	}
	//부모가 반복문이면,
	else if(dynamic_cast<Iteration *>(parent)){
		
		iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parent->GetX() + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX());
	}

	//부모가 케이스이면,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE 자리이면,
		if (iteration->GetX() == midX) {
			iteration->CorrectXWidth(dynamic_cast<Block *>(parent)->GetMidX() - iteration->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX());
		}
		//TRUE 자리이면,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//몇 번째 자리인지 구한다.
			while (previousX + (j * previousDivideWidth) != iteration->GetX()) {
				j++;
			}

			//첫번째 자리이면,
			if (j == 0) {
				iteration->CorrectXWidth(parentX - iteration->GetX(), divideWidth);
			}
			else {

				//default 자리이면,
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
	
	//자식들 모두 재귀로 Resizer한다.
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
	//변하기 전 midX값을 구한다.
	Long midX2 = dynamic_cast<Case *>(sCase)->GetMidX();
	//변하기 전 x값을 구한다.
	Long currentX = sCase->GetX();
	
	//부모가 선택문이면,
	if (dynamic_cast<Selection *>(parent)) {
		//FALSE 자리이면,
		if (sCase->GetX() == midX) {
			sCase->CorrectXWidthMidX(dynamic_cast<Selection *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE 자리이면,
		else {
			sCase->CorrectXWidthMidX(parent->GetX() - sCase->GetX(), dynamic_cast<Block *>(parent)->GetMidX() - parentX, x);
		}
	}
	//부모가 반복문이면,
	else if (dynamic_cast<Iteration *>(parent)){
		
		sCase->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
	}

	//부모가 케이스이면,
	else if (dynamic_cast<Case *>(parent)) {
		Long midX2 = dynamic_cast<Case *>(parent)->GetMidX();
		Long count = dynamic_cast<Case *>(parent)->GetCount();
		Long standardWidth = midX2 - parentX;
		Long divideWidth = standardWidth / (count + 1);

		//FALSE 자리이면,
		if (sCase->GetX() == midX) {
			sCase->CorrectXWidthMidX(dynamic_cast<Block *>(parent)->GetMidX() - sCase->GetX(), parentX + parent->GetWidth() - dynamic_cast<Block *>(parent)->GetMidX(), x);
		}
		//TRUE 자리이면,
		else {
			Long j = 0;
			Long previousStandardWidth = midX - previousX;
			Long previousDivideWidth = previousStandardWidth / (count + 1);

			//몇 번째 자리인지 구한다.
			while (previousX + (j * previousDivideWidth) != sCase->GetX()) {
				j++;
			}

			//첫번째 자리이면,
			if (j == 0) {
				sCase->CorrectXWidthMidX(parentX - sCase->GetX(), divideWidth, x);
			}
			else {

				//default 자리이면,
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

	//자식들 모두 재귀로 Resizer한다.
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

