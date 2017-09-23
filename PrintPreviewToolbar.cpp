//PrintPreviewToolbar.cpp
#include "PrintPreviewToolbar.h"
#include "PrintPreview.h"


BEGIN_MESSAGE_MAP(PrintPreviewToolbar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(10, OnPrintToolbarClicked)
	ON_BN_CLICKED(20, OnZoomInToolbarClicked)
	ON_BN_CLICKED(30, OnZoomOutToolbarClicked)
	ON_BN_CLICKED(40, OnExitToolbarClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PrintPreviewToolbar::PrintPreviewToolbar(){
	this->zoom = 5;
}

PrintPreviewToolbar::~PrintPreviewToolbar(){

}

int PrintPreviewToolbar::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);
	this->printPreview = (PrintPreview*)CWnd::GetParent();

	this->printButton.Create("프린트", WS_CHILD | WS_VISIBLE, CRect(350, 10, 450, 50), this, 10);

	this->zoomInButton.Create("확대", WS_CHILD | WS_VISIBLE, CRect(550, 10, 650, 50), this, 20);
	
	this->zoomOutButton.Create("축소", WS_CHILD | WS_VISIBLE, CRect(750, 10, 850, 50), this, 30);
	
	this->exitButton.Create("나가기", WS_CHILD | WS_VISIBLE, CRect(950, 10, 1050, 50), this, 40);
	
	this->Invalidate();
	return 0;
}

void PrintPreviewToolbar::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	CPaintDC dc(this); //원본 DC  

	CRect rect;
	GetClientRect(&rect);

	//메모리 DC 생성
	CDC memDC;
	//그 전 bitMap변수 선언
	CBitmap *pOldBitmap;
	//현재 bitmap 생성
	CBitmap bitmap;

	//메모리 DC 및 BITMAP과 현재 DC의 설정 일치

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	//선언한 그 전 bitMap 변수에 넣어줌
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);


	//흰색으로 초기화
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
	//memDC.StretchBlt(0, 0, rect.Width() * zoom2 / 10, rect.Height() * zoom2 / 10, &memDC, 0, 0, rect2.Width(), rect2.Height(), WHITENESS);

	//메모리DC에 그리기
	DrawPrintPreviewToolbar(&memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//사용된 메모리 DC 및 BITMAP의 삭제
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	ReleaseDC(pDC);

	EndPaint(&Ps);
}

void PrintPreviewToolbar::DrawPrintPreviewToolbar(CDC *pDC){

}

BOOL PrintPreviewToolbar::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}

void PrintPreviewToolbar::OnPrintToolbarClicked(){
	this->printPreview->Print();
}

void PrintPreviewToolbar::OnZoomInToolbarClicked(){
	if (this->zoom < 20){
		this->zoom += 5;
		//this->printPreview->printPreviewZoom = this->zoom;
		this->printPreview->Invalidate();
	}
}

void PrintPreviewToolbar::OnZoomOutToolbarClicked(){
	if (this->zoom > 0){
		this->zoom -= 5;
		//this->printPreview->printPreviewZoom = this->zoom;
		this->printPreview->Invalidate();
	}
}

void PrintPreviewToolbar::OnExitToolbarClicked(){
	this->printPreview->CloseWindow();
}

void PrintPreviewToolbar::OnClose(){
	CWnd::OnClose();
}

