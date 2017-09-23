//Drawer.cpp
#include "Drawer.h"
#include "NSChartForm.h"
#include <iostream>
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "String.h"
#pragma warning(disable:4996)

Drawer::Drawer(){

}

Drawer::Drawer(const Drawer& source){

}

Drawer::~Drawer(){

}


void Drawer::VisitSequence(Sequence* sequence, CDC *pDC){
	CPoint pt[4];
	Long j = 0;
	Long i = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];

	pt[0].x = sequence->GetX();
	pt[0].y = sequence->GetY();
	pt[1].x = sequence->GetX() + sequence->GetWidth();
	pt[1].y = sequence->GetY();
	pt[2].x = sequence->GetX() + sequence->GetWidth();
	pt[2].y = sequence->GetY() + sequence->GetHeight();
	pt[3].x = sequence->GetX();
	pt[3].y = sequence->GetY() + sequence->GetHeight();
	pDC->Polygon(pt, 4);

	//contents를 배열에 복사한다.
	strcpy(cha, sequence->GetContents().c_str());

	//길이를 구한다
	length = strlen(cha);

	//문자열을 출력한다
	while (i < length + 1) {
		//chaInput에 배열에있는 것을 하나씩 넣어준다.
		chaInput[j] = cha[i];
		//cha가 만약 리턴캐리지이면
		if (cha[i] == '\r' || cha[i] == '\0') {

			//chaInput에 넣었던 리턴캐리지를 널문자로 바꿔준다.
			chaInput[j] = '\0';
			//차곡차곡 넣어주었던 chaInput을 텍스트아웃으로 출력시켜준다.
			pDC->TextOut(sequence->GetX() + 25, sequence->GetY() + (sequence->GetHeight() / 3) + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 17;
		}
		j++;
		i++;
	}
	

}

void Drawer::VisitSelection(Selection* selection, CDC *pDC){
	Long x;
	Long y;
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new Drawer;
	CPoint pt[4];
	pt[0].x = selection->GetX();
	pt[0].y = selection->GetY();
	pt[1].x = selection->GetX() + selection->GetWidth();
	pt[1].y = selection->GetY();
	pt[2].x = selection->GetX() + selection->GetWidth();
	pt[2].y = selection->GetY() + selection->GetHeight();
	pt[3].x = selection->GetX();
	pt[3].y = selection->GetY() + selection->GetHeight();
	pDC->Polygon(pt, 4);
	pDC->MoveTo(CPoint(selection->GetX(), selection->GetY()));
	pDC->LineTo(CPoint(selection->GetMidX(), selection->GetMidY()));
	pDC->MoveTo(CPoint(selection->GetX() + selection->GetWidth(), selection->GetY()));
	pDC->LineTo(CPoint(selection->GetMidX(), selection->GetMidY()));
	x = selection->GetMidX() - (selection->GetWidth() / 30);
	y = selection->GetMidY() - (selection->GetMidY() - selection->GetY()) * 2 / 3;

	//contents를 배열에 복사한다.
	strcpy(cha, selection->GetContents().c_str());

	//길이를 구한다
	length = strlen(cha);

	//문자열을 출력한다
	while (i < length + 1) {
		//chaInput에 배열에있는 것을 하나씩 넣어준다.
		chaInput[j] = cha[i];
		//cha가 만약 리턴캐리지이면
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput에 넣었던 리턴캐리지를 널문자로 바꿔준다.
			chaInput[j] = '\0';
			//차곡차곡 넣어주었던 chaInput을 텍스트아웃으로 출력시켜준다.
			pDC->TextOut(x - 55 , y + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 17;
		}
		j++;
		i++;
	}

	pDC->TextOut(selection->GetX() + (selection->GetWidth() / 10), selection->GetY() + ((selection->GetMidY() - selection->GetY()) * 1 / 2), "TRUE");
	pDC->TextOut(selection->GetX() + (selection->GetWidth() * 4 / 5), selection->GetY() + ((selection->GetMidY() - selection->GetY()) * 1 / 2), "FALSE");
	pDC->LineTo(CPoint(selection->GetMidX(), selection->GetY() + selection->GetHeight()));
	pDC->MoveTo(CPoint(selection->GetX(), selection->GetMidY()));
	pDC->LineTo(CPoint(selection->GetX() + selection->GetWidth(), selection->GetMidY()));

	i = 0;
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->Accept(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void Drawer::VisitIteration(Iteration *iteration, CDC *pDC){
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new Drawer;
	CPoint pt[6];
	pt[0].x = iteration->GetX();
	pt[0].y = iteration->GetY();
	pt[1].x = iteration->GetX() + iteration->GetWidth();
	pt[1].y = iteration->GetY();
	pt[2].x = iteration->GetX() + iteration->GetWidth();
	pt[2].y = iteration->GetMidY();
	pt[3].x = iteration->GetMidX();
	pt[3].y = iteration->GetMidY();
	pt[4].x = iteration->GetMidX();
	pt[4].y = iteration->GetY() + iteration->GetHeight();
	pt[5].x = iteration->GetX();
	pt[5].y = iteration->GetY() + iteration->GetHeight();
	pDC->Polygon(pt, 6);

	//contents를 배열에 복사한다.
	strcpy(cha, iteration->GetContents().c_str());

	//길이를 구한다
	length = strlen(cha);

	//문자열을 출력한다
	while (i < length + 1) {
		//chaInput에 배열에있는 것을 하나씩 넣어준다.
		chaInput[j] = cha[i];
		//cha가 만약 리턴캐리지이면
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput에 넣었던 리턴캐리지를 널문자로 바꿔준다.
			chaInput[j] = '\0';
			//차곡차곡 넣어주었던 chaInput을 텍스트아웃으로 출력시켜준다.
			pDC->TextOut(iteration->GetMidX() - (iteration->GetMidX() - iteration->GetX()) / 2, iteration->GetMidY() - (iteration->GetMidY() - iteration->GetY()) * 2 / 3 + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 17;
		}
		j++;
		i++;
	}

	i = 0;
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->Accept(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void Drawer::VisitCase(Case *sCase, CDC *pDC){
	Long i = 0;
	Long k = 0;
	Long v = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new Drawer;
	//Case 구조
	Long j = 0;
	Long x;
	//전체 사각형을 그린다
	CPoint pt[4];
	pt[0].x = sCase->GetX();
	pt[0].y = sCase->GetY();
	pt[1].x = sCase->GetX() + sCase->GetWidth();
	pt[1].y = sCase->GetY();
	pt[2].x = sCase->GetX() + sCase->GetWidth();
	pt[2].y = sCase->GetY() + sCase->GetHeight();
	pt[3].x = sCase->GetX();
	pt[3].y = sCase->GetY() + sCase->GetHeight();
	pDC->Polygon(pt, 4);

	//왼쪽 대각선을 그린다
	pDC->MoveTo(CPoint(sCase->GetX(), sCase->GetY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	//오른쪽 대각선을 그린다.
	pDC->MoveTo(CPoint(sCase->GetX() + sCase->GetWidth(), sCase->GetY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	//전체 사각형을 둘로 나눈다
	pDC->MoveTo(CPoint(sCase->GetX(), sCase->GetMidY()));
	pDC->LineTo(CPoint(sCase->GetX() + sCase->GetWidth(), sCase->GetMidY()));
	//TRUE, FALSE 구간을 나눈다
	pDC->MoveTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetY() + sCase->GetHeight()));
	//조건문, TRUE, FALSE 글씨를 적는다

	//contents를 배열에 복사한다.
	strcpy(cha, sCase->GetContents().c_str());

	//길이를 구한다
	length = strlen(cha);

	//문자열을 출력한다
	while (i < length + 1) {
		//chaInput에 배열에있는 것을 하나씩 넣어준다.
		chaInput[j] = cha[i];
		//cha가 만약 리턴캐리지이면
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput에 넣었던 리턴캐리지를 널문자로 바꿔준다.
			chaInput[j] = '\0';
			//차곡차곡 넣어주었던 chaInput을 텍스트아웃으로 출력시켜준다.
			pDC->TextOut(sCase->GetX() + (sCase->GetWidth() / 2), (sCase->GetY() + (sCase->GetMidY() - sCase->GetY()) / 5) + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 17;
		}
		j++;
		i++;
	}

	pDC->TextOut(sCase->GetX() + (sCase->GetWidth() / 10), sCase->GetY() + ((sCase->GetMidY() - sCase->GetY()) * 1 / 2), "TRUE");
	pDC->TextOut(sCase->GetMidX() + (((sCase->GetX() + sCase->GetWidth()) - sCase->GetMidX()) / 2), sCase->GetY() + ((sCase->GetMidY() - sCase->GetY()) * 1 / 2), "FALSE");

	//midY2를 구한다
	Long midY2 = sCase->GetMidY() + 30/*(sCase->GetHeight() / 5)*/;
	pDC->MoveTo(CPoint(sCase->GetX(), midY2));
	pDC->LineTo(CPoint(sCase->GetMidX(), midY2));

	Long standardWidth = sCase->GetMidX() - sCase->GetX();
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//반복해서 케이스문을 그린다.
	x = sCase->GetX();
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x, sCase->GetMidY()));
		pDC->LineTo(CPoint(x, sCase->GetY() + sCase->GetHeight()));

		//contents를 배열에 복사한다.
		strcpy(cha, sCase->GetCaseString(j).c_str());

		//길이를 구한다
		length = strlen(cha);

		i = 0;
		v = 0;
		k = 0;
		//문자열을 출력한다
		while (i < length + 1) {
			//chaInput에 배열에있는 것을 하나씩 넣어준다.
			chaInput[v] = cha[i];
			//cha가 만약 리턴캐리지이면
			if (cha[i] == '\r' || cha[i] == '\0') {
				//chaInput에 넣었던 리턴캐리지를 널문자로 바꿔준다.
				chaInput[v] = '\0';
				//차곡차곡 넣어주었던 chaInput을 텍스트아웃으로 출력시켜준다.
				pDC->TextOut(x + (divideWidth / 2), sCase->GetMidY() + ((midY2 - sCase->GetMidY()) / 2) + k, chaInput);
				//chaInput을 초기화시켜준다.
				v = -1;
				i++;
				k = k + 17;
			}
			v++;
			i++;
		}
		x += divideWidth;
		j++;
	}
	pDC->TextOut((x - divideWidth) + (divideWidth / 6), sCase->GetMidY() + ((midY2 - sCase->GetMidY()) / 3), "default");

	i = 0;
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->Accept(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}

}


Drawer& Drawer::operator=(const Drawer& source){

	return *this;
}