//Collector.h

#ifndef _COLLECTOR_H
#define _COLLECTOR_H
#include "Visitor.h"

class Structure;
class CopyStructure;

class Collector: public Visitor{

public:
	Collector();
	Collector(const Collector& source);
	~Collector();
	Collector& operator=(const Collector& source);
	virtual void VisitSequence(Sequence *sequence, NSChart *nsChart, Structure *structure);
	virtual void VisitSelection(Selection *selection, NSChart *nsChart, Structure *structure);
	virtual void VisitIteration(Iteration *iteration, NSChart *nsChart, Structure *structure);
	virtual void VisitCase(Case *sCase, NSChart *nsChart, Structure *structure);

};

#endif //_COLLECTOR_H