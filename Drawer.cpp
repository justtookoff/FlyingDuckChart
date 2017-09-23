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
			pDC->TextOut(sequence->GetX() + 25, sequence->GetY() + (sequence->GetHeight() / 3) + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
			pDC->TextOut(x - 55 , y + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
			pDC->TextOut(iteration->GetMidX() - (iteration->GetMidX() - iteration->GetX()) / 2, iteration->GetMidY() - (iteration->GetMidY() - iteration->GetY()) * 2 / 3 + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
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
	//Case ����
	Long j = 0;
	Long x;
	//��ü �簢���� �׸���
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

	//���� �밢���� �׸���
	pDC->MoveTo(CPoint(sCase->GetX(), sCase->GetY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	//������ �밢���� �׸���.
	pDC->MoveTo(CPoint(sCase->GetX() + sCase->GetWidth(), sCase->GetY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	//��ü �簢���� �ѷ� ������
	pDC->MoveTo(CPoint(sCase->GetX(), sCase->GetMidY()));
	pDC->LineTo(CPoint(sCase->GetX() + sCase->GetWidth(), sCase->GetMidY()));
	//TRUE, FALSE ������ ������
	pDC->MoveTo(CPoint(sCase->GetMidX(), sCase->GetMidY()));
	pDC->LineTo(CPoint(sCase->GetMidX(), sCase->GetY() + sCase->GetHeight()));
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
			pDC->TextOut(sCase->GetX() + (sCase->GetWidth() / 2), (sCase->GetY() + (sCase->GetMidY() - sCase->GetY()) / 5) + k, chaInput);
			//chaInput�� �ʱ�ȭ�����ش�.
			j = -1;
			i++;
			k = k + 17;
		}
		j++;
		i++;
	}

	pDC->TextOut(sCase->GetX() + (sCase->GetWidth() / 10), sCase->GetY() + ((sCase->GetMidY() - sCase->GetY()) * 1 / 2), "TRUE");
	pDC->TextOut(sCase->GetMidX() + (((sCase->GetX() + sCase->GetWidth()) - sCase->GetMidX()) / 2), sCase->GetY() + ((sCase->GetMidY() - sCase->GetY()) * 1 / 2), "FALSE");

	//midY2�� ���Ѵ�
	Long midY2 = sCase->GetMidY() + 30/*(sCase->GetHeight() / 5)*/;
	pDC->MoveTo(CPoint(sCase->GetX(), midY2));
	pDC->LineTo(CPoint(sCase->GetMidX(), midY2));

	Long standardWidth = sCase->GetMidX() - sCase->GetX();
	Long divideWidth = standardWidth / (sCase->GetCount() + 1);

	//�ݺ��ؼ� ���̽����� �׸���.
	x = sCase->GetX();
	j = 0;
	while (j <= sCase->GetCount()) {

		pDC->MoveTo(CPoint(x, sCase->GetMidY()));
		pDC->LineTo(CPoint(x, sCase->GetY() + sCase->GetHeight()));

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
				pDC->TextOut(x + (divideWidth / 2), sCase->GetMidY() + ((midY2 - sCase->GetMidY()) / 2) + k, chaInput);
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