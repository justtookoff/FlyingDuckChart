//AutoCollector.cpp

#include "AutoCollector.h"
#include "Sequence.h"
#include "Iteration.h"
#include "Selection.h"
#include "Case.h"
#include "NSChart.h"

AutoCollector::AutoCollector(){

}

AutoCollector::AutoCollector(const AutoCollector& source){

}

AutoCollector::~AutoCollector(){

}

AutoCollector& AutoCollector::operator=(const AutoCollector& source){

	return *this;
}

void AutoCollector::VisitSequence(Sequence *sequence, Long maxWidth, NSChart *nsChart){

	if (sequence->GetParentStructure() == nsChart){

		sequence->AutoCorrect(0, 0, maxWidth, sequence->GetHeight());
	}
	else{
	
		if (dynamic_cast<Selection*>(sequence->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��
			if (dynamic_cast<Selection*>(sequence->GetParentStructure())->GetMidX() > sequence->GetX()){

				sequence->AutoCorrect(0, 0, dynamic_cast<Selection*>(sequence->GetParentStructure())->GetMidX() - sequence->GetX(), sequence->GetHeight());
				//selectedStructure�� �ڽ��� ������
				//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�
			}
			//�θ��� midX�� selectedStructure�� x ��ǥ�� ������
			else{

				sequence->AutoCorrect(0, 0, dynamic_cast<Selection*>(sequence->GetParentStructure())->GetX() + dynamic_cast<Selection*>(sequence->GetParentStructure())->GetWidth() - dynamic_cast<Selection*>(sequence->GetParentStructure())->GetMidX(), sequence->GetHeight());
				//selectedStructure�� �ڽ��� ������
				//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�

			}


		}
		//�θ� iteration�϶�
		if (dynamic_cast<Iteration*>(sequence->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��


			sequence->AutoCorrect(0, 0, dynamic_cast<Iteration*>(sequence->GetParentStructure())->GetX() + dynamic_cast<Iteration*>(sequence->GetParentStructure())->GetWidth() - dynamic_cast<Iteration*>(sequence->GetParentStructure())->GetMidX(), sequence->GetHeight());
			//selectedStructure�� �ڽ��� ������
			//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�

		}

		if (dynamic_cast<Case*>(sequence->GetParentStructure())){

			Case *parent = dynamic_cast<Case*>(sequence->GetParentStructure());
			Long standardWidth = parent->GetMidX() - parent->GetX();
			Long divideWidth = standardWidth / (parent->GetCount() + 1);

			if (parent->GetMidX() == sequence->GetX()){
			
				sequence->AutoCorrect(0, 0, parent->GetX() + parent->GetWidth() - parent->GetMidX(), sequence->GetHeight());
			}
			else if (sequence->GetX() == parent->GetX() + (parent->GetCount() * divideWidth)){
			
				sequence->AutoCorrect(0, 0, parent->GetMidX() - (parent->GetX() + (parent->GetCount() * divideWidth)), sequence->GetHeight());
			}
			else {
			
				sequence->AutoCorrect(0, 0, divideWidth, sequence->GetHeight());
			}
		}
	}


}

void AutoCollector::VisitSelection(Selection *selection, Long maxWidth, NSChart *nsChart){


	if (selection->GetParentStructure() == nsChart){

		selection->AutoCorrect(0, 0, maxWidth, selection->GetHeight());
	}
	else{

		if (dynamic_cast<Selection*>(selection->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��
			if (dynamic_cast<Selection*>(selection->GetParentStructure())->GetMidX() > selection->GetX()){

				selection->AutoCorrect(0, 0, dynamic_cast<Selection*>(selection->GetParentStructure())->GetMidX() - selection->GetX(), selection->GetHeight());
				
			}
			//�θ��� midX�� selectedStructure�� x ��ǥ�� ������
			else{

				selection->AutoCorrect(0, 0, dynamic_cast<Selection*>(selection->GetParentStructure())->GetX() + dynamic_cast<Selection*>(selection->GetParentStructure())->GetWidth() - dynamic_cast<Selection*>(selection->GetParentStructure())->GetMidX(), selection->GetHeight());
				
			}
		}
		//�θ� iteration�϶�
		if (dynamic_cast<Iteration*>(selection->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��

			selection->AutoCorrect(0, 0, dynamic_cast<Iteration*>(selection->GetParentStructure())->GetX() + dynamic_cast<Iteration*>(selection->GetParentStructure())->GetWidth() - dynamic_cast<Iteration*>(selection->GetParentStructure())->GetMidX(), selection->GetHeight());

		}
		if (dynamic_cast<Case*>(selection->GetParentStructure())){

			Case *parent = dynamic_cast<Case*>(selection->GetParentStructure());
			Long standardWidth = parent->GetMidX() - parent->GetX();
			Long divideWidth = standardWidth / (parent->GetCount() + 1);

			if (parent->GetMidX() == selection->GetX()){

				selection->AutoCorrect(0, 0, parent->GetX() + parent->GetWidth() - parent->GetMidX(), selection->GetHeight());
			}
			else if (selection->GetX() == parent->GetX() + (parent->GetCount() * divideWidth)){

				selection->AutoCorrect(0, 0, parent->GetMidX() - (parent->GetX() + (parent->GetCount() * divideWidth)), selection->GetHeight());
			}
			else {

				selection->AutoCorrect(0, 0, divideWidth, selection->GetHeight());
			}
		}
	}
	Long i = 0;
	Visitor *visitor = new AutoCollector;
	//selectedStructure�� �ڽ��� ������
	//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�
	while (i < selection->GetLength()){
	
		selection->GetChild(i)->Accept(visitor, maxWidth, nsChart);
		i++;
	}

	if (visitor != 0){
	
		delete visitor;
	}

}

void AutoCollector::VisitIteration(Iteration *iteration, Long maxWidth, NSChart *nsChart){


	if (iteration->GetParentStructure() == nsChart){

		iteration->AutoCorrect(0, 0, maxWidth, iteration->GetHeight());
	}
	else{

		if (dynamic_cast<Selection*>(iteration->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��
			if (dynamic_cast<Selection*>(iteration->GetParentStructure())->GetMidX() > iteration->GetX()){

				iteration->AutoCorrect(0, 0, dynamic_cast<Selection*>(iteration->GetParentStructure())->GetMidX() - iteration->GetX(), iteration->GetHeight());

			}
			//�θ��� midX�� selectedStructure�� x ��ǥ�� ������
			else{

				iteration->AutoCorrect(0, 0, dynamic_cast<Selection*>(iteration->GetParentStructure())->GetX() + dynamic_cast<Selection*>(iteration->GetParentStructure())->GetWidth() - dynamic_cast<Selection*>(iteration->GetParentStructure())->GetMidX(), iteration->GetHeight());

			}
		}
		//�θ� iteration�϶�
		if (dynamic_cast<Iteration*>(iteration->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��

			iteration->AutoCorrect(0, 0, dynamic_cast<Iteration*>(iteration->GetParentStructure())->GetX() + dynamic_cast<Iteration*>(iteration->GetParentStructure())->GetWidth() - dynamic_cast<Iteration*>(iteration->GetParentStructure())->GetMidX(), iteration->GetHeight());

		}
		if (dynamic_cast<Case*>(iteration->GetParentStructure())){

			Case *parent = dynamic_cast<Case*>(iteration->GetParentStructure());
			Long standardWidth = parent->GetMidX() - parent->GetX();
			Long divideWidth = standardWidth / (parent->GetCount() + 1);

			if (parent->GetMidX() == iteration->GetX()){

				iteration->AutoCorrect(0, 0, parent->GetX() + parent->GetWidth() - parent->GetMidX(), iteration->GetHeight());
			}
			else if (iteration->GetX() == parent->GetX() + (parent->GetCount() * divideWidth)){

				iteration->AutoCorrect(0, 0, parent->GetMidX() - (parent->GetX() + (parent->GetCount() * divideWidth)), iteration->GetHeight());
			}
			else {

				iteration->AutoCorrect(0, 0, divideWidth, iteration->GetHeight());
			}
		}
	}
	Long i = 0;
	Visitor *visitor = new AutoCollector;
	//selectedStructure�� �ڽ��� ������
	//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�
	while (i < iteration->GetLength()){

		iteration->GetChild(i)->Accept(visitor, maxWidth, nsChart);
		i++;
	}

	if (visitor != 0){

		delete visitor;
	}

}


void AutoCollector::VisitCase(Case *sCase, Long maxWidth, NSChart *nsChart){

	if (sCase->GetParentStructure() == nsChart){

		sCase->AutoCorrect(0, 0, maxWidth, sCase->GetHeight());
	}
	else{

		if (dynamic_cast<Selection*>(sCase->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��
			if (dynamic_cast<Selection*>(sCase->GetParentStructure())->GetMidX() > sCase->GetX()){

				sCase->AutoCorrect(0, 0, dynamic_cast<Selection*>(sCase->GetParentStructure())->GetMidX() - sCase->GetX(), sCase->GetHeight());

			}
			//�θ��� midX�� selectedStructure�� x ��ǥ�� ������
			else{

				sCase->AutoCorrect(0, 0, dynamic_cast<Selection*>(sCase->GetParentStructure())->GetX() + dynamic_cast<Selection*>(sCase->GetParentStructure())->GetWidth() - dynamic_cast<Selection*>(sCase->GetParentStructure())->GetMidX(), sCase->GetHeight());

			}
		}
		//�θ� iteration�϶�
		if (dynamic_cast<Iteration*>(sCase->GetParentStructure())){

			//�θ��� midX�� selectedStructure�� x ��ǥ���� Ŭ��

			sCase->AutoCorrect(0, 0, dynamic_cast<Iteration*>(sCase->GetParentStructure())->GetX() + dynamic_cast<Iteration*>(sCase->GetParentStructure())->GetWidth() - dynamic_cast<Iteration*>(sCase->GetParentStructure())->GetMidX(), sCase->GetHeight());

		}
		if (dynamic_cast<Case*>(sCase->GetParentStructure())){

			Case *parent = dynamic_cast<Case*>(sCase->GetParentStructure());
			Long standardWidth = parent->GetMidX() - parent->GetX();
			Long divideWidth = standardWidth / (parent->GetCount() + 1);

			if (parent->GetMidX() == sCase->GetX()){

				sCase->AutoCorrect(0, 0, parent->GetX() + parent->GetWidth() - parent->GetMidX(), sCase->GetHeight());
			}
			else if (sCase->GetX() == parent->GetX() + (parent->GetCount() * divideWidth)){

				sCase->AutoCorrect(0, 0, parent->GetMidX() - (parent->GetX() + (parent->GetCount() * divideWidth)), sCase->GetHeight());
			}
			else {

				sCase->AutoCorrect(0, 0, divideWidth, sCase->GetHeight());
			}
		}
	}
	Long i = 0;
	Visitor *visitor = new AutoCollector;
	//selectedStructure�� �ڽ��� ������
	//�ڽĵ鵵 ����� ���̸�ŭ �ٽ� ���̸� �����ش�
	while (i < sCase->GetLength()){

		sCase->GetChild(i)->Accept(visitor, maxWidth, nsChart);
		i++;
	}

	if (visitor != 0){

		delete visitor;
	}

}