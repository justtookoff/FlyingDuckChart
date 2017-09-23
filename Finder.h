//Finder.h
#ifndef _FINDER_H
#define _FINDER_H
#include "Visitor.h"

class Structure;
class SelectedStructure;
class GroupStructure;
class GroupSelectedStructure;

class Finder : public Visitor {
public :
	Finder();
	Finder(const Finder& source);
	~Finder();
	Finder& operator=(const Finder& source);
	virtual void VisitSequence(Sequence *sequence, Structure* *indexStructure, Long x, Long y);
	virtual void VisitSelection(Selection *selection, Structure* *indexStructure, Long x, Long y);
	virtual void VisitIteration(Iteration *iteration, Structure* *indexStructure, Long x, Long y);
	virtual void VisitCase(Case *sCase, Structure* *indexStructure, Long x, Long y);

	//드래그해서 다중선택
	virtual void VisitSequence(Sequence *sequence, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	virtual void VisitSelection(Selection *selection, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	virtual void VisitIteration(Iteration *iteration, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	virtual void VisitCase(Case *sCase, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);


};
#endif // !_FINDER_H

