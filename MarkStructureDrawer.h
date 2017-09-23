//MarkStructureDrawer.h
#ifndef _MARKSTRUCTUREDRAWER_H
#define _MARKSTRUCTUREDRAWER_H
#include <afxext.h>

typedef signed long int Long;

class Structure;
class MarkStructureDrawer{

public:

	MarkStructureDrawer();
	MarkStructureDrawer(const MarkStructureDrawer& source);
	~MarkStructureDrawer();
	MarkStructureDrawer& operator=(const MarkStructureDrawer& source);
	void DrawMarkStructure(Long checkingResult, CRectTracker rect, Structure *markStructure, CDC *pDC);
};
#endif //_MARKSTRUCTUREDRAWER_H