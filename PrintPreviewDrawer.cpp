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
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
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

	//contents�� �迭�� �����Ѵ�.
	strcpy(cha, sequence->GetContents().c_str());

	//���̸� ���Ѵ�
	length = strlen(cha);

	//���ڿ��� ����Ѵ�
	while (i < length + 1) {

		//chaInput�� �迭���ִ� ���� �ϳ��� �־��ش�.
		chaInput[j] = cha[i];

		//cha�� ���� ����ĳ�����̸�
		if (cha[i] == '\r' || cha[i] == '\0') {

			//chaInput�� �־��� ����ĳ������ �ι��ڷ� �ٲ��ش�.
			chaInput[j] = '\0';
			//�������� �־��־��� chaInput�� �ؽ�Ʈ�ƿ����� ��½����ش�.
			pDC->TextOut(printPreviewX + 6 + topLeftX, printPreviewY + (sequence->GetHeight() / 10) + topLeftY + k, chaInput);

			//chaInput�� �ʱ�ȭ�����ش�.
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
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
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
	
	//�簢�� �׸���
	pt[0].x = printPreviewX + topLeftX;
	pt[0].y = printPreviewY + topLeftY;

	pt[1].x = printPreviewX + selection->GetWidth() / 3 + topLeftX;
	pt[1].y = printPreviewY + topLeftY;

	pt[2].x = printPreviewX + selection->GetWidth() / 3 + topLeftX;
	pt[2].y = printPreviewY + selection->GetHeight() / 3 + topLeftY;

	pt[3].x = printPreviewX + topLeftX;
	pt[3].y = printPreviewY + selection->GetHeight() / 3 + topLeftY;
	pDC->Polygon(pt, 4);
	//�߰��� �׸���
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(selection->GetMidX() / 3 + topLeftX, selection->GetMidY() / 3 + topLeftY));
	pDC->MoveTo(CPoint(printPreviewX + selection->GetWidth() / 3 + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(selection->GetMidX() / 3 + topLeftX, selection->GetMidY() / 3 + topLeftY));

	x = selection->GetMidX() / 3 - ((selection->GetWidth() / 3) / 30) + topLeftX;
	y = selection->GetMidY() / 3 - ((selection->GetMidY() / 3) - printPreviewY) * 2 / 3 + topLeftY;

	//contents�� �迭�� �����Ѵ�.
	strcpy(cha, selection->GetContents().c_str());

	//���̸� ���Ѵ�
	length = strlen(cha);

	//���ڿ��� ����Ѵ�
	while (i < length + 1) {
		//chaInput�� �迭���ִ� ���� �ϳ��� �־��ش�.
		chaInput[j] = cha[i];
		//cha�� ���� ����ĳ�����̸�
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput�� �־��� ����ĳ������ �ι��ڷ� �ٲ��ش�.
			chaInput[j] = '\0';
			//�������� �־��־��� chaInput�� �ؽ�Ʈ�ƿ����� ��½����ش�.
			pDC->TextOut(x - 20, y - 3 + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
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

	//contents�� �迭�� �����Ѵ�.
	strcpy(cha, iteration->GetContents().c_str());

	//���̸� ���Ѵ�
	length = strlen(cha);

	//���ڿ��� ����Ѵ�
	while (i < length + 1) {
		//chaInput�� �迭���ִ� ���� �ϳ��� �־��ش�.
		chaInput[j] = cha[i];
		//cha�� ���� ����ĳ�����̸�
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput�� �־��� ����ĳ������ �ι��ڷ� �ٲ��ش�.
			chaInput[j] = '\0';
			//�������� �־��־��� chaInput�� �ؽ�Ʈ�ƿ����� ��½����ش�.
			pDC->TextOut(iteration->GetMidX() / 3 - (iteration->GetMidX() / 3 - printPreviewX) / 2 + topLeftX, 
				iteration->GetMidY() / 3 - (iteration->GetMidY() / 3 - printPreviewY) * 2 / 3 + k + topLeftY, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
	myfont.CreateFont(6, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
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
	//Case ����
	Long j = 0;
	Long x;

	Long printPreviewX = sCase->GetX() / 3;
	Long printPreviewY = sCase->GetY() / 3;
	Long printPreviewMidX = sCase->GetMidX() / 3;
	Long printPreviewMidY = sCase->GetMidY() / 3;
	Long printPreviewWidth = sCase->GetWidth() / 3;
	Long printPreviewHeight = sCase->GetHeight() / 3;

	//��ü �簢���� �׸���
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

	//���� �밢���� �׸���
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	//������ �밢���� �׸���.
	pDC->MoveTo(CPoint(printPreviewX + printPreviewWidth + topLeftX, printPreviewY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	//��ü �簢���� �ѷ� ������
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, printPreviewMidY + topLeftY));
	pDC->LineTo(CPoint(printPreviewX + printPreviewWidth + topLeftX, printPreviewMidY + topLeftY));
	//TRUE, FALSE ������ ������
	pDC->MoveTo(CPoint(printPreviewMidX + topLeftX, printPreviewMidY + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, printPreviewY + printPreviewHeight + topLeftY));
	//���ǹ�, TRUE, FALSE �۾��� ���´�

	//contents�� �迭�� �����Ѵ�.
	strcpy(cha, sCase->GetContents().c_str());

	//���̸� ���Ѵ�
	length = strlen(cha);

	//���ڿ��� ����Ѵ�
	while (i < length + 1) {
		//chaInput�� �迭���ִ� ���� �ϳ��� �־��ش�.
		chaInput[j] = cha[i];
		//cha�� ���� ����ĳ�����̸�
		if (cha[i] == '\r' || cha[i] == '\0') {
			//chaInput�� �־��� ����ĳ������ �ι��ڷ� �ٲ��ش�.
			chaInput[j] = '\0';
			//�������� �־��־��� chaInput�� �ؽ�Ʈ�ƿ����� ��½����ش�.
			pDC->TextOut(printPreviewX + (printPreviewWidth / 2) + topLeftX, (printPreviewY + (printPreviewMidX - printPreviewY) / 30) + k + topLeftY, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
			j = -1;
			i++;
			k = k + 6;
		}
		j++;
		i++;
	}

	pDC->TextOut(printPreviewX + (printPreviewWidth / 10) + topLeftX, printPreviewY + ((printPreviewMidY - printPreviewY) * 1 / 2) + topLeftY, "TRUE");
	pDC->TextOut(printPreviewMidX + (((printPreviewX + printPreviewWidth) - printPreviewMidX) / 2) + topLeftX, printPreviewY + ((printPreviewMidY - printPreviewY) * 1 / 2) + topLeftY, "FALSE");
	
	//midY2�� ���Ѵ�
	Long midY2 = printPreviewMidY + 15;
	pDC->MoveTo(CPoint(printPreviewX + topLeftX, midY2 + topLeftY));
	pDC->LineTo(CPoint(printPreviewMidX + topLeftX, midY2 + topLeftY));

	Long standardWidth = printPreviewMidX - printPreviewX;
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//�ݺ��ؼ� ���̽����� �׸���.
	x = printPreviewX;
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x + topLeftX, printPreviewMidY + topLeftY));
		pDC->LineTo(CPoint(x + topLeftX, printPreviewY + printPreviewHeight + topLeftY));

		//contents�� �迭�� �����Ѵ�.
		strcpy(cha, sCase->GetCaseString(j).c_str());

		//���̸� ���Ѵ�
		length = strlen(cha);

		i = 0;
		v = 0;
		k = 0;
		//���ڿ��� ����Ѵ�
		while (i < length + 1) {
			//chaInput�� �迭���ִ� ���� �ϳ��� �־��ش�.
			chaInput[v] = cha[i];
			//cha�� ���� ����ĳ�����̸�
			if (cha[i] == '\r' || cha[i] == '\0') {
				//chaInput�� �־��� ����ĳ������ �ι��ڷ� �ٲ��ش�.
				chaInput[v] = '\0';
				//�������� �־��־��� chaInput�� �ؽ�Ʈ�ƿ����� ��½����ش�.
				pDC->TextOut(x + (divideWidth / 2) + topLeftX, printPreviewMidY + ((midY2 - printPreviewMidY) / 2) + k  + topLeftY, chaInput);
				//chaInput�� �ʱ�ȭ�����ش�.
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