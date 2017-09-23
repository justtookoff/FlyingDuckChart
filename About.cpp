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

	CPaintDC dc(this); //원본 DC  

	CRect rect;
	GetDlgItem(IDC_STATIC_ICON)->GetClientRect(&rect);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_ABOUTICON);

	//참고 : pDC 에는 CPaintDC 객체의 주소가 넘어온다. 즉, client 영역 dc

	CDC memDC;  //메모리 dc(장치와 연결되어 있지 않은 dc)
	memDC.CreateCompatibleDC(pDC);  //메모리dc를 client dc와 호환
	CBitmap *pOldBit = memDC.SelectObject(&bitmap); //메모리 dc가 비트맵객체 선택 
	//-> 메모리 dc가 선택한 비트맵은 메모리 dc의 윈도우 역활을 함.

	BITMAP bm;  //비트맵 정보 기억 구조체 생성
	bitmap.GetBitmap(&bm);  //비트맵 정보 얻어옴

	//case 1 : BitBle - 지정된 창 크기에 맞게
	pDC->BitBlt( //비트맵을 1:1로 출력
		50, 50, rect.right, rect.bottom,   //비트맵이 출력될 client 영역
		&memDC, 0, 0,   //메모리 dc가 선택한 비트맵 좌측상단 x,y 부터 출력
		SRCCOPY  //비트맵을 목적지에 기존 내용위에 복사
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
