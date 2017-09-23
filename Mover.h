//Mover.h
#ifndef _MOVER_H
#define _MOVER_H
#include "Visitor.h"

class Mover : public Visitor {
public :
	Mover();
	Mover(const Mover& source);
	~Mover();
	Mover& operator=(const Mover& source);
	virtual void VisitSequence(Sequence *sequence, Long x, Long y);
	virtual void VisitSelection(Selection *selection, Long x, Long y);
	virtual void VisitIteration(Iteration *iteration, Long x, Long y);
	virtual void VisitCase(Case *sCase, Long x, Long y);
};

#endif // !_MOVER_H

