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

	//PrintPreview�� �θ�� ���´�.
	this->printPreviewScrollBar = (PrintPreviewScrollBar*)CWnd::GetParent();

	//PrintPreview���� ��������� NSChart PrintPreviewScrollBar�� ���� �����´�.
	this->printPreviewNSChart = this->printPreviewScrollBar->transferNSChart;


	this->Invalidate();

	return 0;
}

void PrintPreviewBitmap::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	CRect rect;
	GetWindowRect(&rect);

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

	DrawPrintPreviewBitmap(&memDC, rect);

	//���Ȯ��
	Long zoom2 = this->printPreviewScrollBar->transferZoom + 5;
	pDC->StretchBlt(0, 0, rect.Width() * zoom2 / 10, rect.Height() * zoom2 / 10, &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	pDC->SetStretchBltMode(COLORONCOLOR);

	//���� �޸� DC �� BITMAP�� ����
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	ReleaseDC(pDC);

	EndPaint(&Ps);
}

void PrintPreviewBitmap::DrawPrintPreviewBitmap(CDC *pDC, CRect rect){
	//�����͸� �Ҵ��Ѵ�.
	this->visitor = new PrintPreviewDrawer;

	CRect partRect(0, 0, rect.Width(), rect.Height());

	//��ȣ�� �����ϰ� �Ѵ�.
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

	//�Ҵ������Ѵ�.
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