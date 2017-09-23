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

	this->printButton.Create("����Ʈ", WS_CHILD | WS_VISIBLE, CRect(350, 10, 450, 50), this, 10);

	this->zoomInButton.Create("Ȯ��", WS_CHILD | WS_VISIBLE, CRect(550, 10, 650, 50), this, 20);
	
	this->zoomOutButton.Create("���", WS_CHILD | WS_VISIBLE, CRect(750, 10, 850, 50), this, 30);
	
	this->exitButton.Create("������", WS_CHILD | WS_VISIBLE, CRect(950, 10, 1050, 50), this, 40);
	
	this->Invalidate();
	return 0;
}

void PrintPreviewToolbar::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	CPaintDC dc(this); //���� DC  

	CRect rect;
	GetClientRect(&rect);

	//�޸� DC ����
	CDC memDC;
	//�� �� bitMap���� ����
	CBitmap *pOldBitmap;
	//���� bitmap ����
	CBitmap bitmap;

	//�޸� DC �� BITMAP�� ���� DC�� ���� ��ġ

	// ȭ�� DC�� ȣȯ�Ǵ� �޸� DC ��ü�� ����
	memDC.CreateCompatibleDC(pDC);

	// ���������� ȭ�� DC�� ȣȯ�Ǵ� Bitmap ����
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	//������ �� �� bitMap ������ �־���
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);


	//������� �ʱ�ȭ
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
	//memDC.StretchBlt(0, 0, rect.Width() * zoom2 / 10, rect.Height() * zoom2 / 10, &memDC, 0, 0, rect2.Width(), rect2.Height(), WHITENESS);

	//�޸�DC�� �׸���
	DrawPrintPreviewToolbar(&memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//���� �޸� DC �� BITMAP�� ����
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

