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
			pDC->TextOut(printX + 25, printY + (printHeight / 4) + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
			pDC->TextOut(x - 55, y + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
			pDC->TextOut(printMidX - (printMidX - printX) / 2, printMidY - (printMidY - printY) * 2 / 3 + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
	//Case ����
	Long j = 0;
	Long x;
	//��ü �簢���� �׸���
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

	//���� �밢���� �׸���
	pDC->MoveTo(CPoint(printX, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	//������ �밢���� �׸���.
	pDC->MoveTo(CPoint(printX + printWidth, printY));
	pDC->LineTo(CPoint(printMidX, printMidY));
	//��ü �簢���� �ѷ� ������
	pDC->MoveTo(CPoint(printX, printMidY));
	pDC->LineTo(CPoint(printX + printWidth, printMidY));
	//TRUE, FALSE ������ ������
	pDC->MoveTo(CPoint(printMidX, printMidY));
	pDC->LineTo(CPoint(printMidX, printY + printHeight));
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
			pDC->TextOut(printX + (printWidth / 2), (printY + (printMidY - printY) / 5) + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
			j = -1;
			i++;
			k = k + 15;
		}
		j++;
		i++;
	}

	pDC->TextOut(printX + (printWidth / 10), printY + (printMidY - printY) * 1 / 2, "TRUE");
	pDC->TextOut(printMidX + (((printX + printWidth) - printMidX) / 2), printY + ((printMidY - printY) * 1 / 2), "FALSE");

	//midY2�� ���Ѵ�
	Long midY2 = printMidY + 100;
	pDC->MoveTo(CPoint(printX, midY2));
	pDC->LineTo(CPoint(printMidX, midY2));

	Long standardWidth = printMidX - printX;
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//�ݺ��ؼ� ���̽����� �׸���.
	x = printX;
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x, printMidY));
		pDC->LineTo(CPoint(x, printY + printHeight));

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
				pDC->TextOut(x + (divideWidth / 2), printMidY + ((midY2 - printMidY) / 3) + k, chaInput);
				//chaInput�� �ʱ�ȭ�����ش�.
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