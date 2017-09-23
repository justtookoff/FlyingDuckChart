//Collector.cpp

#include "Collector.h"
#include "Sequence.h"
#include "Iteration.h"
#include "Case.h"
#include "Selection.h"
#include "NSChart.h"
#include "Structure.h"

Collector::Collector(){

}

Collector::Collector(const Collector& source){

}

Collector::~Collector(){
}

Collector& Collector::operator=(const Collector& source){

	return *this;
}

void Collector::VisitSequence(Sequence *sequence, NSChart *nsChart, Structure *structure){
	Long index;

	index=structure->Search(sequence);


	nsChart->Add(sequence);
	sequence->Correct(500, 0);
	sequence->SetParent(nsChart);

	structure->Delete(index);


}
void Collector::VisitSelection(Selection *selection, NSChart *nsChart, Structure *structure){
	Long index;

	index = structure->Search(selection);
	nsChart->Add(selection);
	selection->Correct(500, 0);
	selection->SetParent(nsChart);
	Long i = 0;
	Visitor *visitor = new Collector;
	while (i !=selection->GetLength()){
	
		selection->GetChild(i)->Accept(visitor, nsChart,selection);
		

	}
	structure->Delete(index);
	if (visitor != 0){
	
		delete visitor;
	}

}
void Collector::VisitIteration(Iteration *iteration, NSChart *nsChart, Structure *structure){
	Long index;

	index = structure->Search(iteration);
	nsChart->Add(iteration);
	iteration->Correct(500, 0);
	iteration->SetParent(nsChart);
	Long i = 0;
	Visitor *visitor = new Collector;
	while (i !=iteration->GetLength()){

		iteration->GetChild(i)->Accept(visitor, nsChart, iteration);
		

	}

	structure->Delete(index);
	if (visitor != 0){

		delete visitor;
	}


}
void Collector::VisitCase(Case *sCase, NSChart *nsChart, Structure *structure){

	Long index;

	index = structure->Search(sCase);
	nsChart->Add(sCase);
	sCase->Correct(500, 0);
	sCase->SetParent(nsChart);
	Long i = 0;
	Visitor *visitor = new Collector;
	while (i != sCase->GetLength()){

		sCase->GetChild(i)->Accept(visitor, nsChart,sCase);
		

	}

	structure->Delete(index);

	if (visitor != 0){

		delete visitor;
	}
}