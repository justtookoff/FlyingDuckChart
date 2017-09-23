//Iteration.cpp
#include "Iteration.h"
#include "Visitor.h"
#pragma warning(disable:4996)

Iteration::Iteration() {

}

Iteration::Iteration(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY) 
	:Block(x, y, width, height, contents, midX, midY){
}

Iteration::Iteration(const Iteration& source)
	:Block(source){
}

Iteration::~Iteration() {

}

Structure* Iteration::Clone(){//prototype pattern
	return new Iteration(*this);
}

void Iteration::Accept(Visitor *visitor, CDC *pDC){//visitor pattern
	visitor->VisitIteration(this, pDC);
}

void Iteration::AcceptPrint(Visitor *visitor, CDC *pDC){
	visitor->VisitIterationPrint(this, pDC);
}
//프린트 미리보기 그리기
void Iteration::Accept(Visitor *visitor, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	visitor->VisitIteration(this, topLeftX, topLeftY, bottomRightX, bottomRightY, pDC);
}

void Iteration::Accept(Visitor *visitor,NSChart *temp, Long(*integers), Long *index, Long *count) {
	visitor->VisitIteration(this, temp, integers, index, count);
}

void Iteration::Accept(Visitor *visitor, Structure* *indexStructure, Long x, Long y) {
	visitor->VisitIteration(this, indexStructure, x, y);
}

void Iteration::Accept(Visitor *visitor, Long x, Long y) {
	visitor->VisitIteration(this, x, y);
}
void Iteration::Accept(Visitor *visitor, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	visitor->VisitIteration(this, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
}

void Iteration::Accept(Visitor *visitor){
	visitor->VisitIteration(this);
}

string Iteration::GetString(){
	char str[1000];
	char cha[1000];
	char scv[1000];
	Long i = 0;
	Long j = 0;
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
	scv[j] = '\0';
	sprintf(str, "%s%s%d%s%d%s%d%s%d%s%s%s%d%s%d", "3", ":", this->x, ":", this->y, ":", this->width, ":", this->height, ":", scv, ":", this->midX, ":", this->midY);
	return str;
}

Iteration& Iteration::operator=(const Iteration& source){
	Block::operator=(source);
	return *this;
}

void Iteration::Correct(Long x, Long y) {
	this->x = this->x + x;
	this->y = this->y + y;
	this->midX += x;
	this->midY += y;
}

void Iteration::CorrectSize(Long x, Long y, Long checkingResult){
	
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

	else if (checkingResult == 8) {
		this->midX += x;
		
	}

}

void Iteration::CorrectContents(string object) {
	this->contents = object;
}


void Iteration::Accept(Visitor *visitor, NSChart *nsChart, Structure *structure){

	visitor->VisitIteration(this, nsChart,structure);

}


void Iteration::Correct(Long x, Long y, Long width, Long height){

	this->x += x;
	this->y += y;
	this->width = width;
	this->height = height;
	
	this->midX = this->GetX() + (this->GetWidth() / 9);
	this->midY = this->GetY() + (this->GetHeight() * 4 / 10);
}

void Iteration::Accept1(Visitor *visitor, Long x, Long midX, Long previousX) {
	visitor->VisitIteration1(this, x, midX, previousX);
}

void Iteration::CorrectXWidth(Long x, Long width) {
	this->x += x;
	this->width = width;
	this->midX = this->midX + x;
}

void Iteration::CorrectWidth(Long width) {
	this->width = width;
}


void Iteration::AutoCorrect(Long x, Long y, Long width, Long height){

	this->x += x;
	this->y += y;
	this->width = width;
	this->height = height;

}



void Iteration::Accept(Visitor *visitor, Long maxWidth, NSChart *nsChart){

	visitor->VisitIteration(this, maxWidth, nsChart);

}