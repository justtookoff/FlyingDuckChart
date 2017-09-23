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

	CBitmap bit;  //비트맵 관리

	if (this->count == 0){
		bit.LoadBitmap(IDB_BITMAP_HELP);  //비트맵 load
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

	//참고 : pDC 에는 CPaintDC 객체의 주소가 넘어온다. 즉, client 영역 dc

	CDC memDC;  //메모리 dc(장치와 연결되어 있지 않은 dc)
	memDC.CreateCompatibleDC(pDC);  //메모리dc를 client dc와 호환
	CBitmap *pOldBit = memDC.SelectObject(&bit); //메모리 dc가 비트맵객체 선택 
	//-> 메모리 dc가 선택한 비트맵은 메모리 dc의 윈도우 역활을 함.

	BITMAP bm;  //비트맵 정보 기억 구조체 생성
	bit.GetBitmap(&bm);  //비트맵 정보 얻어옴

	//case 1 : BitBle - 지정된 창 크기에 맞게
	pDC->BitBlt( //비트맵을 1:1로 출력
	0,0,rC.right, rC.bottom,   //비트맵이 출력될 client 영역
	&memDC, 0, 0,   //메모리 dc가 선택한 비트맵 좌측상단 x,y 부터 출력
	SRCCOPY  //비트맵을 목적지에 기존 내용위에 복사
	);  

	//case 2 : StretchBlt - 창크기를 그림 크기로 조절
	/*
	pDC->StretchBlt( //비트맵을 1:1로 출력
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
