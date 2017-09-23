//PrintPreview.cpp
#include "PrintPreview.h"
#include "FlyingDuckChart.h"
#include "NSChartForm.h"
#include "NSChartFormScrollBar.h"
#include "NSChart.h"
#include "Structure.h"
#include "PrintPreviewToolbar.h"
#include "PrintPreviewScrollBar.h"
#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PrintPreview, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PrintPreview::PrintPreview(){
	this->nsChart = NULL;
	this->printPreviewToolbar = NULL;
	this->printPreviewScrollBar = NULL;
	this->printPreviewZoom = 5;

}

PrintPreview::~PrintPreview(){


}

int PrintPreview::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	CRect rect;
	GetWindowRect(&rect);

	this->hFlyingDuckChart = (FlyingDuckChart *)CFrameWnd::GetParent();

	//NSChart를 복사생성함
	this->nsChart = new NSChart(*(this->hFlyingDuckChart->nsChartFormScrollBar->nsChartForm->nsChart));

	//PrintPreview의 툴바를 생성한다.
	this->printPreviewToolbar = new PrintPreviewToolbar;
	this->printPreviewToolbar->Create(NULL, TEXT("PrintPreviewToolbar"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, rect.Width(), 50), this, 10010);

	//PrintPreview의 비트맵을 올려준다.
	this->printPreviewScrollBar = new PrintPreviewScrollBar;
	this->printPreviewScrollBar->Create(NULL, TEXT("PrintPreviewScrollBar"), WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
		CRect(0, 50, rect.Width(), rect.Height()), this, 10012);


	this->Invalidate();

	return 0;
}

void PrintPreview::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	if (this->printPreviewToolbar->zoom != this->printPreviewZoom){
		this->printPreviewZoom = this->printPreviewToolbar->zoom;
		this->printPreviewScrollBar->Invalidate();
	}

	ReleaseDC(pDC);
	EndPaint(&Ps);
}

void PrintPreview::Print(){
	this->hFlyingDuckChart->Print();
}


void PrintPreview::CloseWindow(){
	this->OnClose();
}

BOOL PrintPreview::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}


void PrintPreview::OnClose(){

	if (this->nsChart != NULL){
		delete this->nsChart;
	}
	if (this->printPreviewToolbar != NULL){
		delete this->printPreviewToolbar;
	}
	if (this->printPreviewScrollBar != NULL){
		delete this->printPreviewScrollBar;
	}

	CFrameWnd::OnClose();
}
