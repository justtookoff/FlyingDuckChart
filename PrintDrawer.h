//PrintDrawer.h
#ifndef _PRINTDRAWER_H
#define _PRINTDRAWER_H
#include "Visitor.h"


class PrintDrawer : public Visitor{

public:
	PrintDrawer();
	PrintDrawer(const PrintDrawer& source);
	PrintDrawer& operator =(const PrintDrawer& source);
	virtual void VisitSequencePrint(Sequence *sequence, CDC *pDC);
	virtual void VisitSelectionPrint(Selection *selection, CDC *pDC);
	virtual void VisitIterationPrint(Iteration *iteration, CDC *pDC);
	virtual void VisitCasePrint(Case *sCase, CDC *pDC);
	~PrintDrawer();
};

#endif// _PRINTDRAWER_H