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
	
	//시작 기호를 찾는다
	while (k < nsChart->GetLength() && flag == 0){

		structure2 = nsChart->GetChild(k);
		if (structure2->GetContents().compare("start") == 0 || structure2->GetContents().compare("START") == 0 || structure2->GetContents().compare("Start") == 0){
			startStructure = nsChart->GetChild(k);
			flag++;
		}
		k++;
	}
	//시작기호가 존재할때
	if (flag > 0){

		//준비 기호를 찾는다
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
	//indexes와 count를 초기화 시킨다
	//배열 초기화
	
	
		(*count) = 0;
	


	char cha[256];
	char cha2[256];
	//준비기호의 문자열을 가져온다
	strcpy(cha, readyStructure->GetContents().c_str());
	//문자열의 길이를 구한다
	Long i = 0;
	Long k = 0;
	Long z = 0;
	Long v = 0;
	
	
	
	//문자열의 길이만큼 반복한다
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
	//분리한 문자배열 사용량 까지 반복한다
	i = 0;
	while (i < *count){
		
		//문자배열의 문자열을 가져온다
		strcpy(cha, indexes[i].c_str());
		k = 0;
		flag = 0;
		//문자열의 길이만큼 반복한다
		while (cha[k] != '\0' && flag == 0){
		
			//문자열의 문자가 입력받은 문자와 같을때
			if (cha[k] == input){
				flag = 1;
				//개수를 올린다, 일치하는 문자열이 들은 첨자를 위치배열에 넣는다
				(*sameWordCount)++;
				sameWordIndexes[z] = i;
				z++;
			}
			k++;

		}
		
		i++;
	}
	
}


