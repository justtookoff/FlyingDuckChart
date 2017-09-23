//PrintPreviewBitmap.cpp
#include "PrintPreviewBitmap.h"
#include "PrintPreviewScrollBar.h"
#include "Visitor.h"
#include "NSChart.h"
#include "PrintPreviewDrawer.h"
#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PrintPreviewBitmap, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PrintPreviewBitmap::PrintPreviewBitmap(){
	this->visitor = NULL;
	this->printPreviewNSChart = NULL;
}

PrintPreviewBitmap::~PrintPreviewBitmap(){

}

int PrintPreviewBitmap::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	//PrintPreview를 부모로 갖는다.
	this->printPreviewScrollBar = (PrintPreviewScrollBar*)CWnd::GetParent();

	//PrintPreview에서 복사생성한 NSChart PrintPreviewScrollBar를 통해 가져온다.
	this->printPreviewNSChart = this->printPreviewScrollBar->transferNSChart;


	this->Invalidate();

	return 0;
}

void PrintPreviewBitmap::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	CRect rect;
	GetWindowRect(&rect);

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

	DrawPrintPreviewBitmap(&memDC, rect);

	//축소확대
	Long zoom2 = this->printPreviewScrollBar->transferZoom + 5;
	pDC->StretchBlt(0, 0, rect.Width() * zoom2 / 10, rect.Height() * zoom2 / 10, &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	pDC->SetStretchBltMode(COLORONCOLOR);

	//사용된 메모리 DC 및 BITMAP의 삭제
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	ReleaseDC(pDC);

	EndPaint(&Ps);
}

void PrintPreviewBitmap::DrawPrintPreviewBitmap(CDC *pDC, CRect rect){
	//비지터를 할당한다.
	this->visitor = new PrintPreviewDrawer;

	CRect partRect(0, 0, rect.Width(), rect.Height());

	//기호를 투명하게 한다.
	pDC->SelectStockObject(NULL_BRUSH);

	CBrush back_brush(RGB(155, 154, 160));
	CBrush page_brush(RGB(255, 255, 255));

	CBrush *p_old_brush = pDC->SelectObject(&back_brush);
	pDC->Rectangle(partRect);
	pDC->SelectObject(p_old_brush);

	p_old_brush = pDC->SelectObject(&page_brush);
	pDC->Rectangle(475, 60, 900, 625);

	Long i = 0;
	while (i < this->printPreviewNSChart->GetLength()){
		Structure *structure = this->printPreviewNSChart->GetChild(i);
		structure->Accept(this->visitor, 475, 60, 900, 625, pDC);
		i++;
	}

	pDC->SelectObject(p_old_brush);

	//할당해제한다.
	if (this->visitor != NULL){
		delete this->visitor;
	}
}

BOOL PrintPreviewBitmap::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}


void PrintPreviewBitmap::OnClose(){
	CWnd::OnClose();
}