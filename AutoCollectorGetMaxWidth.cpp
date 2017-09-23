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

	//��뷮���� �ݺ��Ѵ�
	while (k < selectedStructure->GetLength()){

		//selected�� �θ� ��Ʈ�϶�
		if (selectedStructure->GetAt(k)->GetParentStructure() == nsChart){

			//�� ���̰� �ִ���̺��� Ŭ��
			if (selectedStructure->GetAt(k)->GetWidth() > maxWidth){

				//�ִ���̸� �����Ѵ�
				maxWidth = selectedStructure->GetAt(k)->GetWidth();
			}
		}
		k++;
	}
	k = 0;
	//�ٽ� selec��뷮��ŭ �ݺ��Ѵ�
	while (k < selectedStructure->GetLength()){

		selectedStructure->GetAt(k)->Accept(visitor, maxWidth,nsChart);


		k++;
	}

	if (visitor != 0){
	
		delete visitor;
	}
}