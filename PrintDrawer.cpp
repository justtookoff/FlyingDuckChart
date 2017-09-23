//PrintDrawer.cpp
#include "PrintDrawer.h"
#include "NSChartForm.h"
#include <iostream>
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "String.h"
#pragma warning(disable:4996)

PrintDrawer::PrintDrawer(){

}

PrintDrawer::PrintDrawer(const PrintDrawer& source){

}

PrintDrawer::~PrintDrawer(){

}


void PrintDrawer::VisitSequencePrint(Sequence* sequence, CDC *pDC){
	CPoint pt[4];
	Long j = 0;
	Long i = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];

	Long printX = sequence->GetX() * 4;
	Long printY = sequence->GetY() * 4;
	Long printWidth = sequence->GetWidth() * 4;
	Long printHeight = sequence->GetHeight() * 4;

	pt[0].x = printX;
	pt[0].y = printY;

	pt[1].x = printX + printWidth;
	pt[1].y = printY;

	pt[2].x = printX + printWidth;
	pt[2].y = printY + printHeight;

	pt[3].x = printX;
	pt[3].y = printY + printHeight;
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
			pDC->TextOut(printX + 25, printY + (printHeight / 4) + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 105;
		}
		j++;
		i++;
	}


}

void PrintDrawer::VisitSelectionPrint(Selection* selection, CDC *pDC){
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
	visitor = new PrintDrawer;
	CPoint pt[4];

	Long printX = selection->GetX() * 4;
	Long printY = selection->GetY() * 4;
	Long printWidth = selection->GetWidth() * 4;
	Long printHeight = selection->GetHeight() * 4;
	Long printMidX = selection->GetMidX() * 4;
	Long printMidY = selection->GetMidY() * 4;

	pt[0].x = printX;
	pt[0].y = printY;

	pt[1].x = printX + printWidth;
	pt[1].y = printY;

	pt[2].x = printX + printWidth;
	pt[2].y = printY + printHeight;

	pt[3].x = printX;
	pt[3].y = printY + printHeight;
	pDC->Polygon(pt, 4);

	pDC->MoveTo(CPoint(printX, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	pDC->MoveTo(CPoint(printX + printWidth, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	x = printMidX - (printWidth / 30);
	y = printMidY - (printMidY - printY) * 2 / 3;

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
			pDC->TextOut(x - 55, y + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 15;
		}
		j++;
		i++;
	}

	pDC->TextOut(printX + (printWidth / 10), printY + (printMidY - printY) * 1 / 2, "TRUE");
	pDC->TextOut(printX + (printWidth * 4 / 5), printY + (printMidY - printY) * 1 / 2, "FALSE");

	pDC->LineTo(CPoint(printMidX, printY + printHeight));
	pDC->MoveTo(CPoint(printX, printMidY));
	pDC->LineTo(CPoint(printX + printWidth, printMidY));

	i = 0;
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->AcceptPrint(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void PrintDrawer::VisitIterationPrint(Iteration *iteration, CDC *pDC){
	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new PrintDrawer;
	CPoint pt[6];

	Long printX = iteration->GetX() * 4;
	Long printY = iteration->GetY() * 4;
	Long printWidth = iteration->GetWidth() * 4;
	Long printHeight = iteration->GetHeight() * 4;
	Long printMidX = iteration->GetMidX() * 4;
	Long printMidY = iteration->GetMidY() * 4;

	pt[0].x = printX;
	pt[0].y = printY;

	pt[1].x = printX + printWidth;
	pt[1].y = printY;

	pt[2].x = printX + printWidth;
	pt[2].y = printMidY;

	pt[3].x = printMidX;
	pt[3].y = printMidY;

	pt[4].x = printMidX;
	pt[4].y = printY + printHeight;

	pt[5].x = printX;
	pt[5].y = printY + printHeight;
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
			pDC->TextOut(printMidX - (printMidX - printX) / 2, printMidY - (printMidY - printY) * 2 / 3 + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 15;
		}
		j++;
		i++;
	}

	i = 0;
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->AcceptPrint(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void PrintDrawer::VisitCasePrint(Case *sCase, CDC *pDC){
	Long i = 0;
	Long k = 0;
	Long v = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new PrintDrawer;
	//Case 구조
	Long j = 0;
	Long x;
	//전체 사각형을 그린다
	CPoint pt[4];

	Long printX = sCase->GetX() * 4;
	Long printY = sCase->GetY() * 4;
	Long printWidth = sCase->GetWidth() * 4;
	Long printHeight = sCase->GetHeight() * 4;
	Long printMidX = sCase->GetMidX() * 4;
	Long printMidY = sCase->GetMidY() * 4;

	pt[0].x = printX;
	pt[0].y = printY;

	pt[1].x = printX + printWidth;
	pt[1].y = printY;

	pt[2].x = printX + printWidth;
	pt[2].y = printY + printHeight;

	pt[3].x = printX;
	pt[3].y = printY + printHeight;
	pDC->Polygon(pt, 4);

	//왼쪽 대각선을 그린다
	pDC->MoveTo(CPoint(printX, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	//오른쪽 대각선을 그린다.
	pDC->MoveTo(CPoint(printX + printWidth, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	//전체 사각형을 둘로 나눈다
	pDC->MoveTo(CPoint(printX, printMidY));
	pDC->LineTo(CPoint(printX + printWidth, printMidY));
	//TRUE, FALSE 구간을 나눈다
	pDC->MoveTo(CPoint(printMidX, printMidY));
	pDC->LineTo(CPoint(printMidX, printY + printHeight));
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
			pDC->TextOut(printX + (printWidth / 2), (printY + (printMidY - printY) / 5) + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 15;
		}
		j++;
		i++;
	}

	pDC->TextOut(printX + (printWidth / 10), printY + (printMidY - printY) * 1 / 2, "TRUE");
	pDC->TextOut(printMidX + (((printX + printWidth) - printMidX) / 2), printY + ((printMidY - printY) * 1 / 2), "FALSE");

	//midY2를 구한다
	Long midY2 = printMidY + 100;
	pDC->MoveTo(CPoint(printX, midY2));
	pDC->LineTo(CPoint(printMidX, midY2));

	Long standardWidth = printMidX - printX;
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//반복해서 케이스문을 그린다.
	x = printX;
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x, printMidY));
		pDC->LineTo(CPoint(x, printY + printHeight));

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
				pDC->TextOut(x + (divideWidth / 2), printMidY + ((midY2 - printMidY) / 3) + k, chaInput);
				//chaInput을 초기화시켜준다.
				v = -1;
				i++;
				k = k + 15;
			}
			v++;
			i++;
		}
		x += divideWidth;
		j++;
	}
	pDC->TextOut((x - divideWidth) + (divideWidth / 6), printMidY + ((midY2 - printMidY) / 3), "default");

	i = 0;
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->AcceptPrint(visitor, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}

}


PrintDrawer& PrintDrawer::operator=(const PrintDrawer& source){

	return *this;
}