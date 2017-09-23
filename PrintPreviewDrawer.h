//PrintPreviewDrawer.h
#ifndef _PRINTPREVIEWDRAER_H
#define _PRINTPREVIEWDRAWER_H
#include "Visitor.h"

class PrintPreviewDrawer : public Visitor{

public:
	PrintPreviewDrawer();
	PrintPreviewDrawer(const PrintPreviewDrawer& source);
	~PrintPreviewDrawer();
	virtual void VisitSequence(Sequence *sequence, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	virtual void VisitSelection(Selection *selection, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	virtual void VisitIteration(Iteration *iteration, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	virtual void VisitCase(Case *sCase, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	PrintPreviewDrawer& operator=(const PrintPreviewDrawer& source);
};
#endif // !_PRINTPREVIEWDRAER_H
