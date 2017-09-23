//AutoCollectorGetMaxWidth.cpp

#include "AutoCollectorGetMaxWidth.h"
#include "AutoCollector.h"
#include "SelectedStructure.h"
#include "NSChart.h"


AutoCollectorGetMaxWidth::AutoCollectorGetMaxWidth(){

}

AutoCollectorGetMaxWidth::AutoCollectorGetMaxWidth(const AutoCollectorGetMaxWidth& source){

}

AutoCollectorGetMaxWidth::~AutoCollectorGetMaxWidth(){

}

AutoCollectorGetMaxWidth& AutoCollectorGetMaxWidth::operator=(const AutoCollectorGetMaxWidth& source){

	return *this;
}

void AutoCollectorGetMaxWidth::GetMaxWidth(SelectedStructure *selectedStructure, NSChart *nsChart){

	Long k = 0;
	Long maxWidth = 0;
	Visitor *visitor = new AutoCollector;

	//사용량까지 반복한다
	while (k < selectedStructure->GetLength()){

		//selected의 부모가 차트일때
		if (selectedStructure->GetAt(k)->GetParentStructure() == nsChart){

			//그 길이가 최대길이보다 클때
			if (selectedStructure->GetAt(k)->GetWidth() > maxWidth){

				//최대길이를 변경한다
				maxWidth = selectedStructure->GetAt(k)->GetWidth();
			}
		}
		k++;
	}
	k = 0;
	//다시 selec사용량만큼 반복한다
	while (k < selectedStructure->GetLength()){

		selectedStructure->GetAt(k)->Accept(visitor, maxWidth,nsChart);


		k++;
	}

	if (visitor != 0){
	
		delete visitor;
	}
}