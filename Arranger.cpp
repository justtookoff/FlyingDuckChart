//Arranger.cpp
#include "Arranger.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
Arranger::Arranger(){

}

Arranger::Arranger(const Arranger& source){

}

Arranger::~Arranger(){

}

Arranger& Arranger::operator=(const Arranger& source){
	return *this;
}

void Arranger::VisitSequence(Sequence *sequence){
	
}

void Arranger::VisitSelection(Selection *selection){
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor = new Arranger;
	selection->ArrangeChild();

	while (i < selection->GetLength()){
		structure = selection->GetChild(i);
		structure->Accept(visitor);
		i++;
	}

	if (visitor != 0){
		delete visitor;
	}


}

void Arranger::VisitIteration(Iteration *iteration){
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor = new Arranger;
	iteration->ArrangeChild();

	while (i < iteration->GetLength()){
		structure = iteration->GetChild(i);
		structure->Accept(visitor);
		i++;
	}

	if (visitor != 0){
		delete visitor;
	}

}

void Arranger::VisitCase(Case *sCase){
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor = new Arranger;
	sCase->ArrangeChild();

	while (i < sCase->GetLength()){
		structure = sCase->GetChild(i);
		structure->Accept(visitor);
		i++;
	}

	if (visitor != 0){
		delete visitor;
	}

}