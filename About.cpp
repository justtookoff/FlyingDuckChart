//About.cpp
#include "About.h"
#include <afxcmn.h>
#include <afxdb.h>

BEGIN_MESSAGE_MAP(About, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnConfirmButtonClicked)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_STATIC_VERSION, &About::OnStnClickedStaticVersion)
END_MESSAGE_MAP()


About::About(CWnd *parent)
	:CDialog(About::IDD, parent) {

}

BOOL About::OnInitDialog(){
	CDialog::OnInitDialog();

	this->Invalidate();
	return FALSE;
}

void About::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	CPaintDC dc(this); //���� DC  

	CRect rect;
	GetDlgItem(IDC_STATIC_ICON)->GetClientRect(&rect);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_ABOUTICON);

	//���� : pDC ���� CPaintDC ��ü�� �ּҰ� �Ѿ�´�. ��, client ���� dc

	CDC memDC;  //�޸� dc(��ġ�� ����Ǿ� ���� ���� dc)
	memDC.CreateCompatibleDC(pDC);  //�޸�dc�� client dc�� ȣȯ
	CBitmap *pOldBit = memDC.SelectObject(&bitmap); //�޸� dc�� ��Ʈ�ʰ�ü ���� 
	//-> �޸� dc�� ������ ��Ʈ���� �޸� dc�� ������ ��Ȱ�� ��.

	BITMAP bm;  //��Ʈ�� ���� ��� ����ü ����
	bitmap.GetBitmap(&bm);  //��Ʈ�� ���� ����

	//case 1 : BitBle - ������ â ũ�⿡ �°�
	pDC->BitBlt( //��Ʈ���� 1:1�� ���
		50, 50, rect.right, rect.bottom,   //��Ʈ���� ��µ� client ����
		&memDC, 0, 0,   //�޸� dc�� ������ ��Ʈ�� ������� x,y ���� ���
		SRCCOPY  //��Ʈ���� �������� ���� �������� ����
		);

	memDC.SelectObject(pOldBit);
	bitmap.DeleteObject();

	ReleaseDC(pDC);

	EndPaint(&Ps);
}


void About::OnConfirmButtonClicked(){
	EndDialog(0);
}

void About::OnClose(){
	EndDialog(0);
}

void About::OnStnClickedStaticVersion()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
