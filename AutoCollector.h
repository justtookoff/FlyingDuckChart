//AutoCollector.h

#ifndef _AUTOCOLLECTOR_H
#define _AUTOCOLLECTOR_H
#include "Visitor.h"

class AutoCollector :public Visitor {

public:
	AutoCollector();
	AutoCollector(const AutoCollector& source);
	~AutoCollector();
	AutoCollector& operator =(const AutoCollector& source);
	virtual void VisitSequence(Sequence *sequence, Long maxWidth, NSChart *nsChart);
	virtual void VisitSelection(Selection *selection, Long maxWidth, NSChart *nsChart);
	virtual void VisitIteration(Iteration *iteration, Long maxWidth, NSChart *nsChart);
	virtual void VisitCase(Case *sCase, Long maxWidth,NSChart *nsChart);

};


#endif //_AUTOCOLLECTOR_H