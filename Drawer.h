//Drawer.h
#ifndef _DRAWER_H
#define _DRAWER_H
#include "Visitor.h"


class Drawer : public Visitor{

public:
	Drawer();
	Drawer(const Drawer& source);
	Drawer& operator =(const Drawer& source);
	virtual void VisitSequence(Sequence *sequence, CDC *pDC);
	virtual void VisitSelection(Selection *selection, CDC *pDC);
	virtual void VisitIteration(Iteration *iteration, CDC *pDC);
	virtual void VisitCase(Case *sCase, CDC *pDC);
	~Drawer();
};

#endif// _DRAWER_H