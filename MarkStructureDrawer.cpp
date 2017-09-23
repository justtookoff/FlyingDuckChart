//MarkStructureDrawer.cpp
#include "MarkStructureDrawer.h"
#include "Structure.h"
#include "Iteration.h"
#include "Selection.h"
#include "Case.h"

MarkStructureDrawer::MarkStructureDrawer(){

}

MarkStructureDrawer::MarkStructureDrawer(const MarkStructureDrawer& source){

}

MarkStructureDrawer::~MarkStructureDrawer(){

}

MarkStructureDrawer& MarkStructureDrawer::operator=(const MarkStructureDrawer& source){
	return *this;
}

void MarkStructureDrawer::DrawMarkStructure(Long checkingResult, CRectTracker rect, Structure *markStructure, CDC *pDC){
	Long standardWidth;
	Long midY2;
	Long divideWidth;
	Long v;

	if (markStructure != NULL){

		if (checkingResult == 4){

			rect.m_rect = CRect(markStructure->GetX(), markStructure->GetY(),
				markStructure->GetX() + markStructure->GetWidth(), markStructure->GetY() + 15);//(markStructure->GetHeight() * 2 / 10));
		}
		if (checkingResult == 6){

			rect.m_rect = CRect(markStructure->GetX(), markStructure->GetY() + markStructure->GetHeight() - 15, //(markStructure->GetHeight() * 2 / 10),
				markStructure->GetX() + markStructure->GetWidth(), markStructure->GetY() + markStructure->GetHeight());
		}

		if (checkingResult == 8) {

			rect.m_rect = CRect(dynamic_cast<Iteration*>(markStructure)->GetMidX(), dynamic_cast<Iteration*>(markStructure)->GetMidY(),
				dynamic_cast<Iteration*>(markStructure)->GetX() + dynamic_cast<Iteration*>(markStructure)->GetWidth(),
				dynamic_cast<Iteration*>(markStructure)->GetMidY() - markStructure->GetHeight() / 10);
		}

		if (checkingResult == 9) {

			rect.m_rect = CRect(dynamic_cast<Selection*>(markStructure)->GetX(), 
				dynamic_cast<Selection*>(markStructure)->GetMidY() - ((dynamic_cast<Selection*>(markStructure)->GetMidY() - dynamic_cast<Selection*>(markStructure)->GetY()) / 5), 
				dynamic_cast<Selection*>(markStructure)->GetMidX(), 
				dynamic_cast<Selection*>(markStructure)->GetMidY() - ((dynamic_cast<Selection*>(markStructure)->GetMidY() - dynamic_cast<Selection*>(markStructure)->GetY()) / 5) + (dynamic_cast<Selection*>(markStructure)->GetMidY() - dynamic_cast<Selection*>(markStructure)->GetY()) / 5);
		}

		if (checkingResult == 10) {

			rect.m_rect = CRect(dynamic_cast<Selection*>(markStructure)->GetMidX(), 
				dynamic_cast<Selection*>(markStructure)->GetMidY() - ((dynamic_cast<Selection*>(markStructure)->GetMidY() - dynamic_cast<Selection*>(markStructure)->GetY()) / 5),
				dynamic_cast<Selection*>(markStructure)->GetX() + dynamic_cast<Selection*>(markStructure)->GetWidth(), 
				dynamic_cast<Selection*>(markStructure)->GetMidY());
		}

		if (checkingResult == 11) {
			
			rect.m_rect = CRect(dynamic_cast<Case *>(markStructure)->GetMidX(), dynamic_cast<Case *>(markStructure)->GetMidY() - ((dynamic_cast<Case *>(markStructure)->GetMidY() - dynamic_cast<Case *>(markStructure)->GetY()) / 5),
				dynamic_cast<Case *>(markStructure)->GetX() + dynamic_cast<Case *>(markStructure)->GetWidth(),
				dynamic_cast<Case *>(markStructure)->GetMidY());
		}

		if (checkingResult >= 12) {

			standardWidth = dynamic_cast<Case *>(markStructure)->GetMidX() - dynamic_cast<Case *>(markStructure)->GetX();
			midY2 = dynamic_cast<Case *>(markStructure)->GetMidY() + 30;
			divideWidth = standardWidth / (dynamic_cast<Case *>(markStructure)->GetCount() + 1);
			v = checkingResult - 12;
			rect.m_rect = CRect(dynamic_cast<Case *>(markStructure)->GetX() + divideWidth * v, 
				dynamic_cast<Case *>(markStructure)->GetMidY(), 
				dynamic_cast<Case *>(markStructure)->GetX() + (divideWidth * v) + divideWidth, 
				midY2);
		}


		rect.m_nStyle = CRectTracker::hatchedBorder;
		rect.Draw(pDC);
	}
}