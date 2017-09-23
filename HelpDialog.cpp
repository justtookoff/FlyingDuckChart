//HelpDialog.cpp

#include "HelpDialog.h"

#include <afxcmn.h>
#include <afxdb.h>
#include <string>
using namespace std;


BEGIN_MESSAGE_MAP(HelpDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &HelpDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &HelpDialog::OnBnClickedButton4)
END_MESSAGE_MAP()

HelpDialog::HelpDialog(CWnd *parent)
	:CDialog(HelpDialog::IDD, parent){

	
}


BOOL HelpDialog::OnInitDialog(){

	CDialog::OnInitDialog();
	this->count = 0;



	this->Invalidate();
	return FALSE;

}

void HelpDialog::OnPaint(){

	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	//ReleaseDC(pDC);
	//EndPaint(&Ps);

	CRect rC;
	GetDlgItem(IDC_STATIC_PIC2)->GetClientRect(&rC);

	//CString str;
	//str.Format("IDB_BITMAP%d", this->count + 1);

	CBitmap bit;  //��Ʈ�� ����

	if (this->count == 0){
		bit.LoadBitmap(IDB_BITMAP_HELP);  //��Ʈ�� load
	}
	else if (this->count == 1){
		bit.LoadBitmap(IDB_BITMAP_HELP2);
	}
	else if (this->count == 2){
		bit.LoadBitmap(IDB_BITMAP_HELP3);

	}
	else if (this->count == 3){
		bit.LoadBitmap(IDB_BITMAP_HELP4);
	}
	else if (this->count == 4){
		bit.LoadBitmap(IDB_BITMAP_HELP5);

	}
	else if (this->count == 5){
		bit.LoadBitmap(IDB_BITMAP_HELP6);
	}
	else if (this->count == 6){
		bit.LoadBitmap(IDB_BITMAP_HELP7);
	}
	else if (this->count == 7){
		bit.LoadBitmap(IDB_BITMAP_HELP8);

	}

	//���� : pDC ���� CPaintDC ��ü�� �ּҰ� �Ѿ�´�. ��, client ���� dc

	CDC memDC;  //�޸� dc(��ġ�� ����Ǿ� ���� ���� dc)
	memDC.CreateCompatibleDC(pDC);  //�޸�dc�� client dc�� ȣȯ
	CBitmap *pOldBit = memDC.SelectObject(&bit); //�޸� dc�� ��Ʈ�ʰ�ü ���� 
	//-> �޸� dc�� ������ ��Ʈ���� �޸� dc�� ������ ��Ȱ�� ��.

	BITMAP bm;  //��Ʈ�� ���� ��� ����ü ����
	bit.GetBitmap(&bm);  //��Ʈ�� ���� ����

	//case 1 : BitBle - ������ â ũ�⿡ �°�
	pDC->BitBlt( //��Ʈ���� 1:1�� ���
	0,0,rC.right, rC.bottom,   //��Ʈ���� ��µ� client ����
	&memDC, 0, 0,   //�޸� dc�� ������ ��Ʈ�� ������� x,y ���� ���
	SRCCOPY  //��Ʈ���� �������� ���� �������� ����
	);  

	//case 2 : StretchBlt - âũ�⸦ �׸� ũ��� ����
	/*
	pDC->StretchBlt( //��Ʈ���� 1:1�� ���
		0, 0, rC.right, rC.bottom,
		&memDC, 0, 0, bm.bmWidth, bm.bmHeight,
		SRCCOPY
		);
		*/


	memDC.SelectObject(pOldBit);
	bit.DeleteObject();

}


void HelpDialog::OnClose(){

	
	EndDialog(0);
}


void HelpDialog::OnBnClickedButton2()
{
	if (this->count > 0){
	
		this->count--;
		this->Invalidate();
	}
	

}


void HelpDialog::OnBnClickedButton4()
{
	if (this->count < 7){
	
		this->count++;
		this->Invalidate();
	}
}
