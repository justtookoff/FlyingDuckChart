//CheckReadySequence.cpp

#include "CheckReadySequence.h"
#include "NSChart.h"
#include "Structure.h"
#include <string>
#include "Array.h"
#pragma warning(disable:4996)
using namespace std;

CheckReadySequence::CheckReadySequence(){
	
}

CheckReadySequence::~CheckReadySequence(){

}

CheckReadySequence::CheckReadySequence(const CheckReadySequence& source){

}

CheckReadySequence& CheckReadySequence::operator=(const CheckReadySequence& source){
	return *this;
}


void CheckReadySequence::GetReadySequence(NSChart *nsChart, Structure **readyStructure){


	Structure *startStructure = NULL;
	Structure *structure2 = NULL;
	
	Long z = 0;
	Long k = 0;
	Long flag = 0;
	
	Long flag2 = 0;
	
	//���� ��ȣ�� ã�´�
	while (k < nsChart->GetLength() && flag == 0){

		structure2 = nsChart->GetChild(k);
		if (structure2->GetContents().compare("start") == 0 || structure2->GetContents().compare("START") == 0 || structure2->GetContents().compare("Start") == 0){
			startStructure = nsChart->GetChild(k);
			flag++;
		}
		k++;
	}
	//���۱�ȣ�� �����Ҷ�
	if (flag > 0){

		//�غ� ��ȣ�� ã�´�
		Long x = startStructure->GetX();
		Long y = startStructure->GetY() + startStructure->GetHeight();

		k = 0;
		while (k < nsChart->GetLength() && flag2 == 0){

			structure2 = nsChart->GetChild(k);
			if (structure2->GetX() == x && structure2->GetY() == y){
				*readyStructure = nsChart->GetChild(k);
				flag2++;
			}
			k++;
		}

	}
}

void CheckReadySequence::ReadySequenceStrings(Structure *readyStructure, char input, string*indexes, Long *count, Long *sameWordIndexes, Long *sameWordCount){
	//indexes�� count�� �ʱ�ȭ ��Ų��
	//�迭 �ʱ�ȭ
	
	
		(*count) = 0;
	


	char cha[256];
	char cha2[256];
	//�غ��ȣ�� ���ڿ��� �����´�
	strcpy(cha, readyStructure->GetContents().c_str());
	//���ڿ��� ���̸� ���Ѵ�
	Long i = 0;
	Long k = 0;
	Long z = 0;
	Long v = 0;
	
	
	
	//���ڿ��� ���̸�ŭ �ݺ��Ѵ�
	while (cha[i] != '\0'){

		k = 0;
		while (cha[i] != ',' && cha[i] != '=' && cha[i] != '\0'){
		
			cha2[k] = cha[i];

			k++;
			i++;
		}
		if (cha[i] == '='){
		
			while (cha[i] != ',' && cha[i] != '\0'){
				i++;
			}

			cha2[k] = '\0';
			(*count)++;
			indexes[v] = string(cha2);
			v++;
			//this->strings->Store(this->strings->GetLength(), string(cha2));
			//this->length = this->strings->GetLength();
		}
		else if (cha[i] == ','){
		
			cha2[k] = '\0';
			(*count)++;
			indexes[v] = string(cha2);
			v++;
			//this->strings->Store(this->strings->GetLength(), string(cha2));
			//this->length = this->strings->GetLength();
		}
		else if (cha[i] == '\0'){
			cha2[k] = '\0';
			(*count)++;
			indexes[v] = string(cha2);
			v++;
			//this->strings->Store(this->strings->GetLength(), string(cha2));
			//this->length = this->strings->GetLength();
			i--;
		}

		i++;

	}
	(*sameWordCount) = 0;
	Long flag = 0;
	z = 0;
	//�и��� ���ڹ迭 ��뷮 ���� �ݺ��Ѵ�
	i = 0;
	while (i < *count){
		
		//���ڹ迭�� ���ڿ��� �����´�
		strcpy(cha, indexes[i].c_str());
		k = 0;
		flag = 0;
		//���ڿ��� ���̸�ŭ �ݺ��Ѵ�
		while (cha[k] != '\0' && flag == 0){
		
			//���ڿ��� ���ڰ� �Է¹��� ���ڿ� ������
			if (cha[k] == input){
				flag = 1;
				//������ �ø���, ��ġ�ϴ� ���ڿ��� ���� ÷�ڸ� ��ġ�迭�� �ִ´�
				(*sameWordCount)++;
				sameWordIndexes[z] = i;
				z++;
			}
			k++;

		}
		
		i++;
	}
	
}


