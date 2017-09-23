//PrintPreviewDrawer.cpp
#include "PrintPreviewDrawer.h"
#include "NSChartForm.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "String.h"
#include <iostream>
#pragma warning(disable:4996)

PrintPreviewDrawer::PrintPreviewDrawer(){

}

PrintPreviewDrawer::PrintPreviewDrawer(const PrintPreviewDrawer& source){

}

PrintPreviewDrawer::~PrintPreviewDrawer(){

}

void PrintPreviewDrawer::VisitSequence(Sequence *sequence, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	CFont myfont;
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont = pDC->SelectObject(&myfont);

	CPoint pt[4];
	Long j = 0;
	Long i = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];

	Long printPreviewX = sequence->GetX() / 3;
	Long printPreviewY = sequence->GetY() / 3;

	pt[0].x = printPreviewX + topLeftX;
	pt[0].y = printPreviewY + topLeftY;

	pt[1].x = printPreviewX + sequence->GetWidth() / 3 + topLeftX;
	pt[1].y = printPreviewY + topLeftY;

	pt[2].x = printPreviewX + sequence->GetWidth() / 3 + topLeftX;
	pt[2].y = printPreviewY + sequence->GetHeight() / 3 + topLeftY;

	pt[3].x = printPreviewX + topLeftX;
	pt[3].y = printPreviewY + sequence->GetHeight() / 3 + topLeftY;

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
			pDC->TextOut(printPreviewX + 6 + topLeftX, printPreviewY + (sequence->GetHeight() / 10) + topLeftY + k, chaInput);

			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 6;
		}
		j++;
		i++;
	}

}

void PrintPreviewDrawer::VisitSelection(Selection *selection, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	CFont myfont;
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont = pDC->SelectObject(&myfont);

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
	visitor = new PrintPreviewDrawer;
	CPoint pt[4];

	Long printPreviewX = selection->GetX() / 3;
	Long printPreviewY = selection->GetY() / 3;
	
	//사각형 그리기
	pt[0].x = printPreviewX + topLeftX;
	pt[0].y = printPreviewY + topLeftY;

	pt[1].x = printPreviewX + selection->GetWidth() / 3 + topLeftX;
	pt[1].y = printPreviewY + topLeftY;

	pt[2].x = printPreviewX + selection->GetWidth() / 3 + topLeftX;
	pt[2].y = printPreviewY + selection->GetHeight() / 3 + topLeftY;

	pt[3].x = printPreviewX + topLeftX;
	pt[3].y = printPreviewY + selection->GetHeight() / 3 + topLeftY;
	pDC->Polygon(pt, 4);
	//중간점 그리기
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(selection->GetMidX() / 3 + topLeftX, selection->GetMidY() / 3 + topLeftY));
	pDC->MoveTo(CPoint(printPreviewX + selection->GetWidth() / 3 + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(selection->GetMidX() / 3 + topLeftX, selection->GetMidY() / 3 + topLeftY));

	x = selection->GetMidX() / 3 - ((selection->GetWidth() / 3) / 30) + topLeftX;
	y = selection->GetMidY() / 3 - ((selection->GetMidY() / 3) - printPreviewY) * 2 / 3 + topLeftY;

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
			pDC->TextOut(x - 20, y - 3 + k, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 6;
		}
		j++;
		i++;
	}

	pDC->TextOut(printPreviewX + ((selection->GetWidth() / 3) / 10) + topLeftX, printPreviewY + (((selection->GetMidY() / 3) - printPreviewY) * 1 / 2) + topLeftY, "TRUE");
	pDC->TextOut(printPreviewX + ((selection->GetWidth() / 3) * 4 / 5) + topLeftX, printPreviewY + (((selection->GetMidY() / 3) - printPreviewY) * 1 / 2) + topLeftY, "FALSE");

	pDC->LineTo(CPoint(selection->GetMidX() / 3 + topLeftX, printPreviewY + selection->GetHeight() / 3 + topLeftY));
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, selection->GetMidY() / 3 + topLeftY));
	pDC->LineTo(CPoint(printPreviewX + selection->GetWidth() / 3 + topLeftX, selection->GetMidY() / 3 + topLeftY));

	i = 0;
	while (i < selection->GetLength()) {
		structure = selection->GetChild(i);
		structure->Accept(visitor, topLeftX, topLeftY, bottomRightX, bottomRightY, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void PrintPreviewDrawer::VisitIteration(Iteration *iteration, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	CFont myfont;
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont = pDC->SelectObject(&myfont);

	Long i = 0;
	Long j = 0;
	Long k = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new PrintPreviewDrawer;
	CPoint pt[6];

	Long printPreviewX = iteration->GetX() / 3;
	Long printPreviewY = iteration->GetY() / 3;

	pt[0].x = printPreviewX + topLeftX;
	pt[0].y = printPreviewY + topLeftY;

	pt[1].x = printPreviewX + topLeftX + iteration->GetWidth() / 3;
	pt[1].y = printPreviewY + topLeftY;

	pt[2].x = printPreviewX + topLeftX + iteration->GetWidth() / 3;
	pt[2].y = iteration->GetMidY() / 3 + topLeftY;

	pt[3].x = iteration->GetMidX() / 3 + topLeftX;
	pt[3].y = iteration->GetMidY() / 3 + topLeftY;

	pt[4].x = iteration->GetMidX() / 3 + topLeftX;
	pt[4].y = printPreviewY + topLeftY + iteration->GetHeight() / 3;

	pt[5].x = printPreviewX + topLeftX;
	pt[5].y = printPreviewY + topLeftY + iteration->GetHeight() / 3;
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
			pDC->TextOut(iteration->GetMidX() / 3 - (iteration->GetMidX() / 3 - printPreviewX) / 2 + topLeftX, 
				iteration->GetMidY() / 3 - (iteration->GetMidY() / 3 - printPreviewY) * 2 / 3 + k + topLeftY, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 6;
		}
		j++;
		i++;
	}

	i = 0;
	while (i < iteration->GetLength()) {
		structure = iteration->GetChild(i);
		structure->Accept(visitor, topLeftX, topLeftY, bottomRightX, bottomRightY, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void PrintPreviewDrawer::VisitCase(Case *sCase, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){
	CFont myfont;
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont = pDC->SelectObject(&myfont);

	Long i = 0;
	Long k = 0;
	Long v = 0;
	Long length;
	char cha[64000];
	char chaInput[64000];
	Structure *structure;
	Visitor *visitor;
	visitor = new PrintPreviewDrawer;
	//Case 구조
	Long j = 0;
	Long x;

	Long printPreviewX = sCase->GetX() / 3;
	Long printPreviewY = sCase->GetY() / 3;
	Long printPreviewMidX = sCase->GetMidX() / 3;
	Long printPreviewMidY = sCase->GetMidY() / 3;
	Long printPreviewWidth = sCase->GetWidth() / 3;
	Long printPreviewHeight = sCase->GetHeight() / 3;

	//전체 사각형을 그린다
	CPoint pt[4];
	pt[0].x = printPreviewX + topLeftX;
	pt[0].y = printPreviewY + topLeftY;

	pt[1].x = printPreviewX + printPreviewWidth + topLeftX;
	pt[1].y = printPreviewY + topLeftY;

	pt[2].x = printPreviewX + printPreviewWidth + topLeftX;
	pt[2].y = printPreviewY + printPreviewHeight + topLeftY;

	pt[3].x = printPreviewX + topLeftX;
	pt[3].y = printPreviewY + printPreviewHeight + topLeftY;
	pDC->Polygon(pt, 4);

	//왼쪽 대각선을 그린다
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	//오른쪽 대각선을 그린다.
	pDC->MoveTo(CPoint(printPreviewX + printPreviewWidth + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	//전체 사각형을 둘로 나눈다
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewMidY + topLeftY));
	pDC->LineTo(CPoint(printPreviewX + printPreviewWidth + topLeftX, printPreviewMidY + topLeftY));
	//TRUE, FALSE 구간을 나눈다
	pDC->MoveTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewY + printPreviewHeight + topLeftY));
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
			pDC->TextOut(printPreviewX + (printPreviewWidth / 2) + topLeftX, (printPreviewY + (printPreviewMidX - printPreviewY) / 30) + k + topLeftY, chaInput);
			//chaInput을 초기화시켜준다.
			j = -1;
			i++;
			k = k + 6;
		}
		j++;
		i++;
	}

	pDC->TextOut(printPreviewX + (printPreviewWidth / 10) + topLeftX, printPreviewY + ((printPreviewMidY - printPreviewY) * 1 / 2) + topLeftY, "TRUE");
	pDC->TextOut(printPreviewMidX + (((printPreviewX + printPreviewWidth) - printPreviewMidX) / 2) + topLeftX, printPreviewY + ((printPreviewMidY - printPreviewY) * 1 / 2) + topLeftY, "FALSE");
	
	//midY2를 구한다
	Long midY2 = printPreviewMidY + 15;
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, midY2 + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, midY2 + topLeftY));

	Long standardWidth = printPreviewMidX - printPreviewX;
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//반복해서 케이스문을 그린다.
	x = printPreviewX;
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x + topLeftX, printPreviewMidY + topLeftY));
		pDC->LineTo(CPoint(x + topLeftX, printPreviewY + printPreviewHeight + topLeftY));

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
				pDC->TextOut(x + (divideWidth / 2) + topLeftX, printPreviewMidY + ((midY2 - printPreviewMidY) / 2) + k  + topLeftY, chaInput);
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
	pDC->TextOut((x - divideWidth) + (divideWidth / 6) + topLeftX, printPreviewMidY + ((midY2 - printPreviewMidY) / 3) + topLeftY, "default");

	i = 0;
	while (i < sCase->GetLength()) {
		structure = sCase->GetChild(i);
		structure->Accept(visitor, topLeftX, topLeftY, bottomRightX, bottomRightY, pDC);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}

}

PrintPreviewDrawer& PrintPreviewDrawer::operator=(const PrintPreviewDrawer& source){
	return *this;
}