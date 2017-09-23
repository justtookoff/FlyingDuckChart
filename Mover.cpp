//Mover.cpp
#include "Mover.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"

Mover::Mover() {

}

Mover::Mover(const Mover& source) {

}

Mover::~Mover() {

}

void Mover::VisitSequence(Sequence *sequence, Long x, Long y) {
	sequence->Correct(x, y);
}

void Mover::VisitSelection(Selection *selection, Long x, Long y) {
	Structure *structure;
	Visitor *visitor = new Mover;
	Long i = 0;

	selection->Correct(x, y);
	
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->Accept(visitor, x, y);
		i++;
	}
	
	if (visitor != 0) {
		delete visitor;
	}
}

void Mover::VisitIteration(Iteration *iteration, Long x, Long y) {
	Structure *structure;
	Visitor *visitor = new Mover;
	Long i = 0;

	iteration->Correct(x, y);
	
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->Accept(visitor, x, y);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void Mover::VisitCase(Case *sCase, Long x, Long y) {
	Structure *structure;
	Visitor *visitor = new Mover;
	Long i = 0;

	sCase->Correct(x, y);
	
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->Accept(visitor, x, y);
		i++;
	}
	
	if (visitor != 0) {
		delete visitor;
	}
}

Mover& Mover::operator=(const Mover& source) {
	return *this;
}