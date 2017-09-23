//PrintPreviewScrollBar.cpp
#include "PrintPreviewScrollBar.h"
#include "PrintPreview.h"
#include "PrintPreviewBitmap.h"
#pragma warning(disable:4996)
const int LINESIZE = 8;

BEGIN_MESSAGE_MAP(PrintPreviewScrollBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PrintPreviewScrollBar::PrintPreviewScrollBar(){
	this->printPreviewBitmap = NULL;
}

PrintPreviewScrollBar::~PrintPreviewScrollBar(){
	if (this->printPreviewBitmap != NULL){
		delete this->printPreviewBitmap;
	}
}

int PrintPreviewScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	CRect rect;
	GetWindowRect(&rect);

	//PrintPreview�� �θ�� ���´�.
	this->printPreview = (PrintPreview*)CWnd::GetParent();

	//�θ���PrintPreview���� ��������� MSChart�� �ڽ��� PrintPreviewBitmap�� ���ؼ� �޾ƿ�
	this->transferNSChart = this->printPreview->nsChart;

	//�θ��� PrintPreview���� Ȯ��, ��Ҹ� ���� ������ PrintPreviewBitmap�� ���ؼ� �޾���.
	this->transferZoom = this->printPreview->printPreviewZoom;

	////PrintPreview�� ��Ʈ���� �÷��ش�.
	this->printPreviewBitmap = new PrintPreviewBitmap;
	this->printPreviewBitmap->Create(NULL, TEXT("PrintPreviewBitmap"), WS_CHILD | WS_VISIBLE,
		CRect(0, 0, rect.Width() * 4, rect.Height() * 5.5), this, 10011);

	/////////////////////��ũ��/////////////////////////////////////
	CClientDC dc(this);

	// Cellwidth will be 1 logical screen inch
	this->cellWidth = dc.GetDeviceCaps(LOGPIXELSX);
	// cellheight will be 1 / 5 logical screen inch
	this->cellHeight = dc.GetDeviceCaps(LOGPIXELSY) / 5;
	// number ribbon is half cell width
	this->ribbonWidth = cellWidth / 2;
	// 26 letters + ribbon
	this->viewWidth = (26 * this->cellWidth) + this->ribbonWidth;
	// 100 cells down
	this->viewHeight = this->cellHeight * 100;
	// set the scroll pos
	this->hScrollPos = 0;
	this->vScrollPos = 0;

	///////////////////////��ũ��/////////////////////////////////////

	this->Invalidate();

	return 0;
}

void PrintPreviewScrollBar::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);

	if (this->printPreview->printPreviewZoom != this->transferZoom){
		this->transferZoom = this->printPreview->printPreviewZoom;
		this->printPreviewBitmap->Invalidate();
	}

	//��ũ�ѳ��� ��ŭ ������
	pDC->SetWindowOrg(this->hScrollPos, this->vScrollPos);

	ReleaseDC(pDC);

	EndPaint(&Ps);
}

BOOL PrintPreviewScrollBar::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}

/////////////////////////////////////////��ũ��/////////////////////////////////////////
void PrintPreviewScrollBar::OnSize(UINT type, int x, int y){
	// call base class OnSize
	CWnd::OnSize(type, x, y);
	SCROLLINFO si;

	// ���� ��ũ�ѹ� ���� ������Ʈ
	Long HScrollMax = 0;
	this->hPageSize = 0;
	if (x < this->viewWidth){

		HScrollMax = this->viewWidth - 1;
		this->hPageSize = x;
		this->hScrollPos = min(this->hScrollPos, this->viewWidth - this->hPageSize - 1);
	}

	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = HScrollMax;
	si.nPos = this->hScrollPos;
	si.nPage = this->hPageSize;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	// ���� ��ũ�ѹ� ���� ������Ʈ
	Long VScrollMax = 0;
	this->vPageSize = 0;
	if (y < this->viewHeight){

		VScrollMax = this->viewHeight - 1;
		this->vPageSize = y;
		this->vScrollPos = min(this->vScrollPos, this->viewHeight - this->vPageSize - 1);
	}
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = VScrollMax;
	si.nPos = this->hScrollPos;
	si.nPage = this->vPageSize;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

void PrintPreviewScrollBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	// number of pixels to be scrolled
	Long delta;
	// which sb msg was it. set delta accordingly
	switch (nSBCode){

	case SB_LINEUP:
		delta = -LINESIZE;
		break;
	case SB_PAGEUP:
		delta = -this->vPageSize;
		break;
	case SB_THUMBTRACK:
		delta = static_cast<Long>(nPos)-this->vScrollPos;
		break;
	case SB_PAGEDOWN:
		delta = this->vPageSize;
		break;
	case SB_LINEDOWN:
		delta = LINESIZE;
		break;
	default:

		return;
	}
	Long scrollpos = this->vScrollPos + delta;
	Long maxpos = this->viewHeight - this->vPageSize;
	if (scrollpos < 0) {
		delta = -this->vScrollPos;
	}
	else {
		if (scrollpos > maxpos){
			delta = maxpos - this->vScrollPos;
		}
	}
	// if we need to scroll then do the scrolling
	if (delta != 0){

		this->vScrollPos += delta;
		SetScrollPos(SB_VERT, this->vScrollPos, TRUE);
		ScrollWindow(0, -delta);
	}

	this->printPreviewBitmap->Invalidate(TRUE);
	this->Invalidate();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void PrintPreviewScrollBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	// number of pixels to be scrolled
	Long delta;
	// which sb msg was sent. Set delta accordingly
	switch (nSBCode){

	case SB_LINELEFT:
		delta = -LINESIZE;
		break;
	case SB_PAGELEFT:
		delta = -this->hPageSize;
		break;
	case SB_THUMBTRACK:
		delta = static_cast<Long>(nPos)-this->hScrollPos;
		break;
	case SB_PAGERIGHT:
		delta = this->hPageSize;
		break;
	case SB_LINERIGHT:
		delta = LINESIZE;
		break;
	default:

		return;
	}
	Long scrollpos = this->hScrollPos + delta;
	Long maxpos = this->viewWidth - this->hPageSize;
	if (scrollpos < 0){
		delta = -this->hScrollPos;
	}
	else{
		if (scrollpos > maxpos){
			delta = maxpos - this->hScrollPos;
		}
	}
	// if we need to scroll do the scrolling
	if (delta != 0){

		this->hScrollPos += delta;
		SetScrollPos(SB_HORZ, this->hScrollPos, TRUE);
		ScrollWindow(-delta, 0);
	}
	
	this->printPreviewBitmap->Invalidate(TRUE);
	this->Invalidate();
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

}

BOOL PrintPreviewScrollBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){
	bool up = true;
	int delta = zDelta;
	if (zDelta < 0)
	{
		up = false;
		delta = -delta;
	}
	UINT WheelScrollLines;
	// get from the OS the number of wheelscrolllines. win95 doesnt support this call
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &WheelScrollLines, 0);
	// scroll a page at a time
	if (WheelScrollLines == WHEEL_PAGESCROLL)
	{
		SendMessage(WM_VSCROLL, MAKEWPARAM((up) ? SB_PAGEUP : SB_PAGEDOWN, 0), 0);
	}
	else
		// or scroll numlines
	{
		int numlines = (delta * WheelScrollLines) / WHEEL_DELTA;
		while (numlines--)
			SendMessage(WM_VSCROLL, MAKEWPARAM((up) ? SB_LINEUP : SB_LINEDOWN, 0), 0);
	}
	return TRUE;

	//���Ȯ��
#if 0
	if (IsCTRLPressed() == TRUE) {

		if (zDelta > 0){ //���콺 ���� ���� �ø���

			if (this->zoom < 10){

				this->zoom++;
			}
		}
		else{ //���콺 ���� �Ʒ��� ���� ��

			if (this->zoom > 0){

				this->zoom--;

			}
		}
		this->Invalidate();
	}
#endif


}
/////////////////////////////////////////��ũ��/////////////////////////////////////////


void PrintPreviewScrollBar::OnClose(){

	CWnd::OnClose();
}