//Arranger.h
#ifndef _ARRANGER_H
#define _ARRANGER_H
#include "Visitor.h"

class Structure;

class Arranger : public Visitor{

public:

	Arranger();
	Arranger(const Arranger& source);
	~Arranger();
	Arranger& operator=(const Arranger& source);
	virtual void VisitSequence(Sequence *sequence);
	virtual void VisitSelection(Selection *selection);
	virtual void VisitIteration(Iteration *iteration);
	virtual void VisitCase(Case *sCase);


};
#endif //_ARRANGERR_H