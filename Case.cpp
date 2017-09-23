//Case.cpp
#include "Case.h"
#include "Visitor.h"
#include "String.h"
#pragma warning(disable:4996)

Case::Case() {
	
}

Case::Case(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY, Long count)
	:Block(x, y, width, height, contents, midX, midY) {
	this->count = count;

	Long i = 0;
	while (i < this->count) {
		if (this->caseStrings.GetLength() < this->caseStrings.GetCapacity()) {
			this->caseStrings.Store(i, "");
		}
		else if(this->caseStrings.GetLength() >= this->caseStrings.GetCapacity()){
			this->caseStrings.AppendFromRear("");
		}
		i++;
	}
	
		
}

Case::Case(const Case& source)
	: Block(source) {

	this->caseStrings = source.caseStrings;
	this->count = source.count;
	this->length = source.length;
	this->capacity = source.capacity;

}

Case::~Case() {
	
}

Structure* Case::Clone(){//prototype pattern
	return new Case(*this);
}

string Case::GetString(){
	char str[1000];
	char cha[1000];
	char scv[1000];
	Long i = 0;
	Long j = 0;
	Long z = 0;
	Long length;
	strcpy(cha, this->contents.c_str());

	length = strlen(cha);

	while (i < length) {
		if (cha[i] != '\r') {
			scv[j] = cha[i];
			j++;
		}
		else if (cha[i] == '\r') {
			i += 1;
			scv[j] = '~';
			j++;
		}
		i++;
	}
	if (length == 0) {
		scv[j] = ' ';
		j++;
	}

	/////////////////////////////////8.4 Case Load. Save
	i = 0;
	while (i < this->count){
		scv[j] = '$';
		j++;
		strcpy(cha, this->GetCaseString(i).c_str());
		z = 0;
		length = strlen(cha);
		while (z < length){
			if (cha[z] != '\r') {
				scv[j] = cha[z];
				j++;
			}
			else if (cha[z] == '\r') {
				z += 1;
				scv[j] = '~';
				j++;
			}
			z++;

		}
		if (length == 0) {
			scv[j] = ' ';
			j++;
		}
		i++;
	}
	//////////////////////////////////////////////////////////

	scv[j] = '\0';
	sprintf(str, "%s%s%d%s%d%s%d%s%d%s%s%s%d%s%d%s%d", "4", ":", this->x, ":", this->y, ":", this->width, ":", this->height, ":", scv, ":", this->midX, ":", this->midY, ":", this->count);
	return str;
}

void Case::Accept(Visitor *visitor, CDC *pDC){//visitor pattern
	visitor->VisitCase(this, pDC);
}

void Case::AcceptPrint(Visitor *visitor, CDC *pDC){
	visitor->VisitCasePrint(this, pDC);
}

//프린트 미리보기 그리기
void Case::Accept(Visitor *visitor, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	visitor->VisitCase(this, topLeftX, topLeftY, bottomRightX, bottomRightY, pDC);
}

void Case::Accept(Visitor *visitor, NSChart *temp, Long(*integers), Long *index, Long *count) {
	visitor->VisitCase(this, temp, integers, index, count);
}

void Case::Accept(Visitor *visitor, Structure* *indexStructure, Long x, Long y) {
	visitor->VisitCase(this, indexStructure, x, y);
}

void Case::Accept(Visitor *visitor, Long x, Long y) {
	visitor->VisitCase(this, x, y);
}

void Case::Accept(Visitor *visitor, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	visitor->VisitCase(this, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
}

void Case::Accept(Visitor *visitor){
	visitor->VisitCase(this);
}

Case& Case::operator=(const Case& source){
	Block::operator=(source);
	return *this;
}

void Case::Correct(Long x, Long y) {
	this->x = this->x + x;
	this->y = this->y + y;
	this->midX += x;
	this->midY += y;
	
}

void Case::CorrectSize(Long x, Long y, Long checkingResult){
	if (checkingResult == 0){

		this->x = this->x + x;
		this->y = this->y + y;
		this->width -= x;
		this->height -= y;
		
	}
	else if (checkingResult == 1){


		this->y += y;
		this->width += x;
		this->height -= y;


	}
	else if (checkingResult == 3){
		this->x += x;
		this->width -= x;
		this->height += y;

	}
	else if (checkingResult == 2){


		this->width += x;
		this->height += y;


	}
	else if (checkingResult == 4){

		this->y += y;
		this->height -= y;

	}
	else if (checkingResult == 5){

		this->width += x;
	}
	else if (checkingResult == 6){

		this->height += y;
	}
	else if (checkingResult == 7){

		this->x += x;
		this->width -= x;
		
	}
	else if (checkingResult == 10) {

		this->midX += x;
		
	}

}

void Case::CorrectCaseCount(Long count) {
	this->count = count;
}

Long Case::CaseStringModify(Long index, string object) {
	
	return this->caseStrings.Modify(index, object);
	
}

void Case::CorrectContents(string object) {
	this->contents = object;
}


void Case::Accept(Visitor *visitor, NSChart *nsChart, Structure *structure){

	visitor->VisitCase(this, nsChart,structure);

}

void Case::Correct(Long x, Long y,Long width,Long height) {
	this->x = this->x + x;
	this->y = this->y + y;
	this->width = width;
	this->height = height;
	
	this->midX = (this->GetX() + this->GetWidth() - (this->GetWidth() / (this->GetCount() + 2)));
	this->midY = this->GetY() + (this->GetHeight() / 5);
}

void Case::Accept1(Visitor *visitor, Long x, Long midX, Long previousX) {
	visitor->VisitCase1(this, x, midX, previousX);
}

void Case::CorrectXWidthMidX(Long x, Long width, Long midX) {
	this->x += x;
	this->width = width;
	this->midX += midX;
}

void Case::CorrectWidth(Long width) {
	this->width = width;
}

void Case::AutoCorrect(Long x, Long y, Long width, Long height){

	this->x += x;
	this->y += y;
	this->width = width;
	this->height = height;

}


void Case::Accept(Visitor *visitor, Long maxWidth, NSChart *nsChart){

	visitor->VisitCase(this, maxWidth, nsChart);

}