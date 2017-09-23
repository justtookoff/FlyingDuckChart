//NSChartForm.cpp
#include "NSChartForm.h"
#include "NSChart.h"
#include "Structure.h"
#include "Sequence.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"
#include "Drawer.h"
#include "PrintPreviewDrawer.h"
#include "Edit.h"
#include "Saver.h"
#include "Mover.h"
#include "StructureFactory.h"
#include "Tokenizer.h"
#include "SelectedStructure.h"
#include "MouseLButton.h"
#include "JustSelectStructure.h"
#include "CtrlSelectStructure.h"
#include "ResizeStructure.h"
#include "CheckingPoint.h"
#include "CopyStructure.h"
#include "KeyBoard.h"
#include "Copy.h"
#include "Paste.h"
#include "Esc.h"
#include "Delete.h"
#include "Cut.h"
#include "MarkStructure.h"
#include "Visitor.h"
#include "SelectedDrawer.h"
#include "MarkStructureDrawer.h"
#include "Collector.h"
#include "PickStructure.h"
#include "InsertStructure.h"
#include "GroupStructure.h"
#include "GroupSelectedStructure.h"
#include "GroupCopyStructure.h"
#include "AutoCollectorGetMaxWidth.h"
#include "NSChartFormScrollBar.h"
#include "FlyingDuckChart.h"
#include "GroupIndexes.h"
#include "HelpDialog.h"
#include <fstream>
#include <string>
using namespace std;

#pragma warning(disable:4996)
#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))
#define IsSHIFTPressed() ( 0x8000 ==(GetKeyState(VK_SHIFT) & 0x8000 ))

BEGIN_MESSAGE_MAP(NSChartForm, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

NSChartForm::NSChartForm() {
	this->nsChart = NULL;
	this->selectedStructure = NULL;
	this->copyStructure = NULL;
	this->groupStructure = NULL;
	this->groupSelectedStructure = NULL;
	this->groupCopyStructure = NULL;
	this->groupIndexes = NULL;
}

NSChartForm::~NSChartForm() {

}



int NSChartForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);
	this->nsChartFormScrollBar = (NSChartFormScrollBar*)CWnd::GetParent();
	
	this->nsChart = new NSChart;
	this->selectedStructure = new SelectedStructure;
	this->copyStructure = new CopyStructure;
	this->groupStructure = new GroupStructure;
	this->groupSelectedStructure = new GroupSelectedStructure;
	this->groupCopyStructure = new GroupCopyStructure;
	this->groupIndexes = new GroupIndexes;

	this->currentState = 0;
	this->previousState = 0;
	this->form = NULL;
	this->preview = NULL;
	this->pickStructure = NULL;
	this->pickGroupStructure = NULL;
	this->groupIndex = -1;

	this->editCopy = new char[64000];
	this->editOpen = false;


	return 0;
}

void NSChartForm::OnPaint() {
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
	//memDC.StretchBlt(0, 0, rect.Width() * zoom2 / 10, rect.Height() * zoom2 / 10, &memDC, 0, 0, rect2.Width(), rect2.Height(), WHITENESS);
	
	//�޸�DC�� �׸���
	DrawImage(&memDC);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//���� �޸� DC �� BITMAP�� ����
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	
	ReleaseDC(pDC);

	EndPaint(&Ps);
}


void NSChartForm::DrawImage(CDC *pDC){

	Long i = 0;
	Long j = 0;
	Structure *structure;
	Visitor *visitor;
	visitor = new Drawer;
	CheckingPoint *checkingPoint = NULL;

	//��ȣ�� �����ϰ� �Ѵ�.
	pDC->SelectStockObject(NULL_BRUSH);

	//���콺�� ��ȣ���̱�
	if (this->preview != NULL){
		this->previewMove = true;
		this->preview->Accept(visitor, pDC);
	}

	//�巡�׸� �ϸ�
	if (this->drag == true){
		this->rect.m_rect = CRect(this->previousX, this->previousY, this->currentX, this->currentY);
		this->rect.m_nStyle = CRectTracker::dottedLine;
		this->rect.Draw(pDC);
	}
	
	i = 0;
	//��ȣ �׸���
	while (i < this->nsChart->GetLength()) {
		structure = this->nsChart->GetChild(i);
		structure->Accept(visitor, pDC);
		i++;
	}

	//������ �� ����� ������ �����ش�.
	if (this->pickStructure != NULL){
		this->pickStructure->Accept(visitor, pDC);
	}

	//������ �� ����� �׷��� ������ �����ش�.
	if (this->pickGroupStructure != NULL) {
		Long j = 0;
		while (j < this->pickGroupStructure->GetLength()) {
			this->pickGroupStructure->GetAt(j)->Accept(visitor, pDC);
			j++;
		}
	}

	//���߼��� �����ֱ�
	SelectedDrawer *selectedDrawer = new SelectedDrawer;
	selectedDrawer->DrawStructure(this->selectedStructure, this->rect, pDC);

	//�׷콺Ʈ���� �����ֱ�
	selectedDrawer->DrawGroupStructure(this->groupSelectedStructure, this->rect, pDC);

	//������ ��ġ�� �����ִ� �۾�
	MarkStructureDrawer *markStructureDrawer = new MarkStructureDrawer;
	markStructureDrawer->DrawMarkStructure(this->checkingResult, this->rect, this->markStructure, pDC);


	if (visitor != 0) {
		delete visitor;
	}

	if (selectedDrawer != 0){
		delete selectedDrawer;
	}

	if (markStructureDrawer != 0){
		delete markStructureDrawer;
	}

}

void NSChartForm::ClipBoardCopy(){
	CMetaFileDC mfdc;
	
	if (mfdc.CreateEnhanced(NULL, NULL, NULL, NULL)) {
		
		//��ȣ�� �����ϰ� �Ѵ�.
		mfdc.SelectStockObject(NULL_BRUSH);

		ClipBoardDrawImage(&mfdc);
		
		HENHMETAFILE hmf;
		if ((hmf = mfdc.CloseEnhanced())) {
			if (OpenClipboard())
			{
				EmptyClipboard();
				SetClipboardData(CF_ENHMETAFILE, hmf);
				CloseClipboard();
			}

			::DeleteEnhMetaFile(hmf);
		}
		
	}
	
}

void NSChartForm::ClipBoardDrawImage(CDC *pDC){
	Long i = 0;
	Visitor *visitor;
	visitor = new Drawer;
	Structure *structure;
	
	Group group;
	
	while (i < this->copyStructure->GetLength()){
		structure = this->copyStructure->GetAt(i);
		structure->Accept(visitor, pDC);
		i++;
	}
	
	i = 0;
	while (i < this->groupCopyStructure->GetLength()){
		group = this->groupCopyStructure->GetAt(i);
		Long j = 0;
		while (j < group.GetLength()){
			structure = group.GetAt(j);
			structure->Accept(visitor, pDC);
			j++;
		}
		i++;
	}

	if (visitor != NULL) {
		delete visitor;
	}
}

void NSChartForm::OnLButtonDown(UINT nFlags, CPoint point) {
	Structure *structure = 0;
	MouseLButton *mouseLButton = new MouseLButton(new CtrlSelectStructure); //��Ʈ�� ���� ���û��¸� ����Ʈ������
	CRectTracker rect;
	Long i = 0;
	Structure *inGroupStructure = 0;
	this->result = -1;
	
	//1.1 ���� ��ǥ�� ���Ѵ�.
	this->currentX = point.x;
	this->currentY = point.y;
	this->previousX = this->currentX;
	this->previousY = this->currentY;

	//1.2 ��ȣ�� ã�´�.
	structure = this->nsChart->Find(this->currentX, this->currentY);
	
	//��Ʈ�� ����
	if (nFlags & MK_CONTROL && this->preview == NULL) {
		
		//�׷� ��Ʈ�� �������� 
		mouseLButton->SelectGroup(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, structure, &inGroupStructure);

		//ã�� structure�� �׷쿡 ������ �׳� ��Ʈ�� ����
		if (inGroupStructure == 0) {
			mouseLButton->Select(this->selectedStructure, structure, point.x, point.y, &result);
		}
	}
	//�׳� ����
	else if (this->preview == NULL) {
		
		mouseLButton->SetState(new JustSelectStructure);
		//�׷� ��������
		mouseLButton->SelectGroup(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, structure, &inGroupStructure);
		
		//ã�� structure�� �׷쿡 ������ �׳� ����
		if (inGroupStructure == 0) {
			mouseLButton->Select(this->selectedStructure, structure, point.x, point.y, &result);
		}
		
	}
	
	//�׷��� ���õǾ� �ִٸ�,
	if (this->groupSelectedStructure->GetLength() > 0) {
		this->groupStructureMove = true;
	}
		
	if (mouseLButton != NULL) {
		delete mouseLButton;
	}

	//�ʱ�ȭ�Ѵ�
	this->Invalidate();

	if (structure != 0 && this->preview == NULL) {
	rect.m_rect = CRect(structure->GetX(), structure->GetY(), structure->GetX() + structure->GetWidth(), structure->GetY() + structure->GetHeight());
	//������ ������ �� �ְ� ������ǥ�� �����Ѵ�
	if (rect.m_rect.PtInRect(point)){
		this->startPos = point;
		this->startToMove = true;
		
	}

	SetCapture();

	}

	else if (this->selectedStructure->GetLength() == 0 && this->preview == NULL){
		this->drag = true;
	}

	if (this->result != -1) {
		this->startToResize = true;
		this->startToMove = false;

		if (this->result == 0 || this->result == 2){
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		else if (this->result == 1 || this->result == 3){
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		else if (this->result == 4 || this->result == 6){
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		else if (this->result == 5 || this->result == 7 || this->result == 8 || this->result == 9 || this->result == 10){
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}

	}
	else if (structure != 0){
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
	}			
}

void NSChartForm::OnMouseMove(UINT nFlags, CPoint point) {

	Visitor *visitor = new Mover;
	Visitor *visitorCollect = new Collector;
	MouseLButton *mouseLButton = NULL;
	CheckingPoint *checkingPoint = NULL;
	Long i = 0;
	CPoint pos;
	Structure *structure;
	Structure *parentStructure;
	this->currentX = point.x;
	this->currentY = point.y;

	//������ ��ġ�� ã�´�.
	mouseLButton = new MouseLButton(new MarkStructure);

	this->checkingResult = -1;
	this->markStructure = NULL;
		
	mouseLButton->Mark(this->nsChart, this->groupStructure, this->selectedStructure, this->currentX, this->currentY, &this->currentState, &this->previousState, &this->markStructure, &this->checkingResult);

	if (this->currentState - this->previousState == 0){

		this->Invalidate();

	}

	this->endPos = point;
	pos = this->endPos - this->startPos;
	this->startPos = point;

	//���콺�� �����϶� ������ �����δ�
	if (this->editOpen == false && this->startToMove == true) {

		//����
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		this->selectedStructure->Arrange();
		if (this->selectedStructure->GetLength() == 1) {
			mouseLButton->SetState(new PickStructure);
			mouseLButton->Pick(this->nsChart, this->selectedStructure, &this->pickStructure);
		}

		this->rect.m_rect.SetRect(this->rect.m_rect.TopLeft() + pos, this->rect.m_rect.BottomRight() + pos);
		this->rect.m_rect.TopLeft().x;

		while (i < this->selectedStructure->GetLength()) {
			this->currentStructure = this->selectedStructure->GetAt(i);
			this->currentStructure->Correct(pos.x, pos.y);
			this->Invalidate();
			i++;
		}
		//�׷콺Ʈ���ĸ� ����
		if (this->groupStructureMove == true) {
			i = 0;
			this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
			if (this->groupSelectedStructure->GetLength() == 1) {
				mouseLButton->SetState(new PickStructure);
				mouseLButton->PickGroup(this->nsChart, this->groupSelectedStructure, &this->pickGroupStructure);
			}

			while (i < this->groupSelectedStructure->GetLength()) {
				Long y = 0;
				this->currentGroup = this->groupSelectedStructure->GetAt(i);
				this->currentGroup->Arrange();
				while (y < this->currentGroup->GetLength()) {
					Structure *findStructure = this->currentGroup->GetAt(y);
					findStructure->Correct(pos.x, pos.y);
					this->Invalidate();
					y++;
				}
				i++;
			}
		}
	}

	//���콺�� �����϶� ���λ����ϴ� preview Structure�� ����ٴ�
	else if (this->previewMove == true && this->preview != NULL){
		this->preview->Correct(this->currentX - this->preview->GetX(), this->currentY - this->preview->GetY());
		this->Invalidate();
	}
	
	//���� ũ�� ������ �Ҷ�
	else if (this->editOpen == false && this->startToResize == true && this->selectedStructure->GetLength() == 1){

		mouseLButton->SetState(new ResizeStructure);
		mouseLButton->Resize(this->selectedStructure, pos.x, pos.y, this->result);
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		if (this->result == 0 || this->result == 2){
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		else if (this->result == 1 || this->result == 3){
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		else if (this->result == 4 || this->result == 6){
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		else if (this->result == 5 || this->result == 7 || this->result == 8 || this->result == 9 || this->result == 10){
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}

		
		i = 0;
		//ũ�� ���������� �ؿ� ���� �ڵ����� ������  ������ ��������
		//visitor = new Mover;
		if (this->result == 2 || this->result == 3 || this->result == 6){
			parentStructure = this->selectedStructure->GetAt(0)->GetParentStructure();
			while (i < dynamic_cast<Block*>(parentStructure)->GetLength()){

				structure = parentStructure->GetChild(i);
				if (structure->GetX() == this->selectedStructure->GetAt(0)->GetX() && 
					structure->GetY() >= this->selectedStructure->GetAt(0)->GetY() && 
					structure != this->selectedStructure->GetAt(0)){

					structure->Accept(visitor, 0, pos.y);
				}
				i++;
			}
		}
		else if (this->result == 0 || this->result == 1 || this->result == 4){

			parentStructure = this->selectedStructure->GetAt(0)->GetParentStructure();
			while (i < dynamic_cast<Block*>(parentStructure)->GetLength()){

				structure = parentStructure->GetChild(i);
				if (structure->GetX() == this->selectedStructure->GetAt(0)->GetX() &&
					structure->GetY() <= this->selectedStructure->GetAt(0)->GetY() && 
					structure != this->selectedStructure->GetAt(0)){

					structure->Accept(visitor, 0, pos.y);
				}
				i++;
			}
		}

		this->Invalidate();
	}
		
	if (mouseLButton != NULL){
		delete mouseLButton;
	}

	if (this->drag == true){
		this->Invalidate();
	}

	if (visitor != NULL){
		delete visitor;
	}

	if (visitorCollect != NULL){
		delete visitorCollect;
	}
}


void NSChartForm::OnLButtonUp(UINT nFlags, CPoint point) {
	Structure *insertStructure = NULL;
	Group *insertGroup = NULL;
	MouseLButton *mouseLButton = NULL;
	Long temp;

	this->currentX = point.x;
	this->currentY = point.y;

	if (this->drag == true && this->previousX != this->currentX && this->previousY != this->currentY) {
		//������ �Ʒ����� ���� ���� �巡�� �� ��
		if (this->previousX > this->currentX && this->previousY > this->currentY){
			temp = this->previousX;
			this->previousX = this->currentX;
			this->currentX = temp;

			temp = this->previousY;
			this->previousY = this->currentY;
			this->currentY = temp;
		}
		//������ ������ ���� �Ʒ��� �巡�� �� ��
		else if (this->previousX > this->currentX && this->previousY < this->currentY){
			temp = this->previousX;
			this->previousX = this->currentX;
			this->currentX = temp;

		}
		//���� �Ʒ����� ������ ���� �巡�� �� ��
		else if (this->previousX < this->currentX && this->previousY > this->currentY){
			temp = this->previousY;
			this->previousY = this->currentY;
			this->currentY = temp;
		}
		this->nsChart->DragFind(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, this->previousX, this->previousY, this->currentX, this->currentY);
	}
	this->drag = false;


	//��ȣ�� ����������
	if (this->startToMove == true || this->previewMove == true || this->groupStructureMove == true) {
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		//1.1 ��ȣ�� ������ ��
		
		//������ �ִ� ��ȣ�� ���� ��
		if (this->pickStructure != NULL){

			insertStructure = this->pickStructure;
		}
		//���ο� ��ȣ�� ���� ��
		else if (this->preview != NULL){
			
			insertStructure = this->preview;

		}
		//�׷��� ���� ��
		if (this->pickGroupStructure != NULL) {

			insertGroup = this->pickGroupStructure;
		}

		// NSChart�� ������ ��,
		if (this->markStructure != NULL){

			mouseLButton = new MouseLButton(new InsertStructure);
			
			//��ȣ�� ������ ��,
			if (insertStructure != NULL && insertGroup == NULL) {

				mouseLButton->Insert(insertStructure, this->markStructure, this->checkingResult);

			}
			
			//�׷츸 ������ ��,
			if (insertGroup != NULL && insertStructure == NULL) {

				mouseLButton->InsertGroup(insertGroup, this->markStructure, this->checkingResult);

			}

			//��ȣ�� �׷��� ���ÿ� ������ ��,
			if (insertStructure != NULL && insertGroup != NULL) {
				
				insertStructure->SetParent(this->nsChart);
				this->nsChart->Add(insertStructure);

				Long i = 0;
				while (i < insertGroup->GetLength()) {
					insertGroup->GetAt(i)->SetParent(this->nsChart);
					this->nsChart->Add(insertGroup->GetAt(i));
					i++;
				}
			}
		
		}

		// �� ���̿� ������ ��,
		else if(this->markStructure == NULL){
			
			//��ȣ�� ������ ��,
			if (insertStructure != NULL && insertGroup == NULL) {
				insertStructure->SetParent(this->nsChart);
				this->nsChart->Add(insertStructure);
			}

			//�׷��� ������ ��,
			if (insertGroup != NULL && insertStructure == NULL) {
				Long i = 0;
				while (i < insertGroup->GetLength()) {
					Structure *parent = insertGroup->GetAt(i)->GetParentStructure();
					if (parent == insertGroup->GetAt(i)){
						insertGroup->GetAt(i)->SetParent(this->nsChart);
						this->nsChart->Add(insertGroup->GetAt(i));
					}
					i++;
				}

			}

			//��ȣ�� �׷��� ���ÿ� ������ ��,
			if (insertStructure != NULL && insertGroup != NULL) {
					
				insertStructure->SetParent(this->nsChart);
				this->nsChart->Add(insertStructure);

				Long i = 0;
				while (i < insertGroup->GetLength()) {
					Structure *parent = insertGroup->GetAt(i)->GetParentStructure();
					if (parent == insertGroup->GetAt(i)){
						insertGroup->GetAt(i)->SetParent(this->nsChart);
						this->nsChart->Add(insertGroup->GetAt(i));
					}
					i++;
				}
			}
			
		}
				
		//������ ��ġ�� �������ش�.
		this->endPos = point;

		this->startToMove = false;
		this->pickStructure = NULL;
		this->preview = NULL;
		this->previewMove = false;
		this->groupStructureMove = false;
		this->pickGroupStructure = NULL;

	}
		
	this->nsChart->Arrange();

	//��ȣ ũ�⸦ ����������
	if (this->startToResize == true){

		this->endPos = point;
		this->startToResize = false;

	}

	if (mouseLButton != NULL){
		delete mouseLButton;
	}

	this->Invalidate();
	//���� ���콺���� ����
	ReleaseCapture();
	
}

void NSChartForm::OnLButtonDblClk(UINT nFlags, CPoint point) {
	
	if (this->form != NULL) {

		this->form->OnClose();

	}

	if (this->selectedStructure->GetLength() == 1) {

		Structure *structure = this->selectedStructure->GetAt(0);
		if (dynamic_cast<Case *>(structure)) {

			if (this->checkingResult >= 12) {
				Long j = this->checkingResult - 12;
				if (j != dynamic_cast<Case *>(structure)->GetCount()) {

					Long standardWidth = dynamic_cast<Case *>(structure)->GetMidX() - dynamic_cast<Case *>(structure)->GetX();
					Long divideWidth = standardWidth / (dynamic_cast<Case *>(structure)->GetCount() + 1);
					Long midY2 = dynamic_cast<Case *>(structure)->GetMidY() + 30;//(dynamic_cast<Case*>(structure)->GetHeight() / 5);
					this->form = new Edit;
					this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE, 
						//Drawer.cpp�� �����ؼ� �׷��ش�.
						CRect(this->selectedStructure->GetAt(0)->GetX() + (divideWidth * j) + (divideWidth / 2),
							dynamic_cast<Case *>(structure)->GetMidY() + (midY2 - dynamic_cast<Case *>(structure)->GetMidY()) / 2,
							this->selectedStructure->GetAt(0)->GetX() + (divideWidth * (j + 1)), 
							midY2), this, 10000);
					//CFrameWnd�� �ִ� ������ �����.
					dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->CaseStringModify(j, "");
					this->form->SetFocus();
				}
			}
			else {
				this->form = new Edit;
				this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE, 
					//Drawer.cpp�� �����ؼ� �׷��ش�.
					CRect(dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetX() + (dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetWidth() / 2),
						(dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetY() + (dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetY()) / 5),
						this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(), 
						this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1/ 2), this, 10000);
				//CFrameWnd�� �ִ� ������ �����.
				this->selectedStructure->GetAt(0)->CorrectContents("");
				this->form->SetFocus();
			}

		}
		else if (dynamic_cast<Iteration*>(structure)){
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE,
				//Drawer.cpp�� �����ؼ� �׷��ش�.
				CRect(dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidX() - (dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidX() - dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetX()) / 2,
					dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidY() - (dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetY()) * 2 / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(),
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1 / 2), this, 10000);
			//CFrameWnd�� �ִ� ������ �����.
			this->selectedStructure->GetAt(0)->CorrectContents("");
			this->form->SetFocus();
		}
		
		else if (dynamic_cast<Selection*>(structure)){
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE,
				//Drawer.cpp�� �����ؼ� �׷��ش�.
				CRect(dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidX() - (dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetWidth() / 30) - 55,
					dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidY() - (dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetY()) * 2 / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(),
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1 / 2), this, 10000);
			//CFrameWnd�� �ִ� ������ �����.
			this->selectedStructure->GetAt(0)->CorrectContents("");
			this->form->SetFocus();
		}

		else {
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE, 
				//Drawer.cpp�� �����ؼ� �׷��ش�.
				CRect(this->selectedStructure->GetAt(0)->GetX() + 25,
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(), 
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight()), this, 10000);
			//CFrameWnd�� �ִ� ������ �����.
			this->selectedStructure->GetAt(0)->CorrectContents("");
			this->form->SetFocus();
		}
	}
}


void NSChartForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Structure *structure = 0;
	KeyBoard *keyBoard = new KeyBoard(new Esc);
	Long i = 0;
	Visitor *visitor = new Collector;

	//�޴����ִ� ��� Ű�ٿ���
	if (nChar == 'x' || nChar == 'X' || nChar == 'c' || nChar == 'C' || nChar == 'v' || nChar == 'V' ||
		nChar == 'a' || nChar == 'A' || nChar == 'g' || nChar == 'G' || nChar == 'u' || nChar == 'U' || nChar == VK_DELETE || nChar=='r' || nChar=='R' || nChar=='n' || nChar=='N'||
		nChar =='o' || nChar=='O' || nChar=='s' || nChar=='S'){
		if (IsCTRLPressed() == TRUE){
			EditMenu(nChar);
		}
		else{
			EditMenu(VK_DELETE);
		}
		
	}

	if (nChar == VK_F1) {
	
		HelpDialog helpDialog;
		helpDialog.DoModal();
	}

	if (nChar == '1' || nChar == '2' || nChar == '3' || nChar == '4' && this->form == NULL){
		StructureMenu(nChar);
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
	}

	//ESC��ư ��������
	if (nChar == VK_ESCAPE){

		keyBoard->Clear(this->copyStructure);

		if (this->preview != NULL){
			delete this->preview;
			this->preview = NULL;
			this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 0;
		}
	}

	//Ctrl + p�� ���������� ����ġ ���̽����� �����ش�.
	if (nChar == 'p' || nChar == 'P') {
		if (IsCTRLPressed() == TRUE) {
			
			if (this->selectedStructure->GetLength() > 0) {
				structure = this->selectedStructure->GetAt(0);
				if (dynamic_cast<Case *>(structure)) {
					this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
					Long count = dynamic_cast<Case *>(structure)->GetCount();
					count++;
					dynamic_cast<Case *>(structure)->CorrectCaseCount(count);
					//�ڽ��� ���ش�.
					Long z = 0;
					while (z != dynamic_cast<Case *>(structure)->GetLength()) {
						dynamic_cast<Case *>(structure)->GetChild(z)->Accept(visitor, this->nsChart, structure);
						
					}
				}
				this->Invalidate();
			}
		}
	}

	//Ctrl + m�� �������� �� ����ġ ���̽����� ���ش�.
	if (nChar == 'm' || nChar == 'M') {
		if (IsCTRLPressed() == TRUE) {
			
			if (this->selectedStructure->GetLength() > 0) {
				structure = this->selectedStructure->GetAt(0);
				if (dynamic_cast<Case *>(structure)) {
					this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
					Long count = dynamic_cast<Case *>(structure)->GetCount();
					if (count > 1) {
						count--;
					}
					dynamic_cast<Case *>(structure)->CorrectCaseCount(count);
					//�ڽ��� ���ش�.
					Long z = 0;
					while (z != dynamic_cast<Case *>(structure)->GetLength()) {
						dynamic_cast<Case *>(structure)->GetChild(z)->Accept(visitor, this->nsChart, structure);
					}
				}
				this->Invalidate();
			}
		}
	}

	//�Ҵ�� keyBoard�� �����Ѵ�
	if (keyBoard != NULL){
		delete keyBoard;
	}

	switch (nChar) {

	case VK_RIGHT:
		
		while (i < this->selectedStructure->GetLength()) {
			this->currentStructure = this->selectedStructure->GetAt(i);
			if (IsCTRLPressed() == TRUE) {
				this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
				this->currentStructure->CorrectSize(1, 0, 5);
			}
			else {
			this->currentStructure->Correct(1, 0);
			}
			i++;
		}

		this->Invalidate();
		break;
	case VK_LEFT:
		
		while (i < this->selectedStructure->GetLength()) {
			this->currentStructure = this->selectedStructure->GetAt(i);
			if (IsCTRLPressed() == TRUE) {
				this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
				this->currentStructure->CorrectSize(-1, 0, 5);
			}
			else {
			this->currentStructure->Correct(-1, 0);
			}
			i++;
		}
		this->Invalidate();
		break;
	case VK_UP:
		while (i < this->selectedStructure->GetLength()) {
			this->currentStructure = this->selectedStructure->GetAt(i);
			if (IsCTRLPressed() == TRUE) {
				this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
				this->currentStructure->CorrectSize(0, -1, 6);
			}
			else {
			this->currentStructure->Correct(0, -1);
			}
			i++;
		}
		this->Invalidate();
		break;
	case VK_DOWN:
		while (i < this->selectedStructure->GetLength()) {
			this->currentStructure = this->selectedStructure->GetAt(i);
			if (IsCTRLPressed() == TRUE) {
				this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
				this->currentStructure->CorrectSize(0, 1, 6);
			}
			else {
			this->currentStructure->Correct(0, 1);
			}
			i++;
		}
		this->Invalidate();
		break;


	default:
		break;
	}
	this->Invalidate();

	if (visitor != NULL) {
		delete visitor;
	}

}


BOOL NSChartForm::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}


void NSChartForm::EditMenu(UINT nChar){
	KeyBoard *keyBoard = new KeyBoard(new Esc);

	//Ctrl + x�� ����������
	if (nChar == 'x' || nChar == 'X'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Cut);
		keyBoard->CutFunction(this->selectedStructure, this->copyStructure, this->nsChart);
		keyBoard->CutGroupFunction(this->groupSelectedStructure, this->groupCopyStructure, this->nsChart);
		this->Invalidate();
		this->ClipBoardCopy();
	}

	//CTRL + C �� ��������
	if (nChar == 'c' || nChar == 'C'){
		
		keyBoard->SetKeyBoardAction(new Copy);
		keyBoard->CopyFunction(this->selectedStructure, this->copyStructure);
		keyBoard->CopyGroupFunction(this->groupSelectedStructure, this->groupCopyStructure);
		this->ClipBoardCopy();
	}

	//CTRL + V�� ��������
	if (nChar == 'v' || nChar == 'V'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Paste);
		//keyBoard->PasteFunction(this->copyStructure, this->currentX, this->currentY, this->nsChart);
		//keyBoard->PasteGroupFunction(this->groupCopyStructure, this->groupStructure, this->currentX, this->currentY, this->nsChart);
		keyBoard->CombinedPasteFunction(this->copyStructure, this->groupCopyStructure, this->groupStructure, this->currentX, this->currentY, this->nsChart);
		this->Invalidate();
				
	}

	//�� ��ȣ�� �����Ѵ�.
	if (nChar == 'a' || nChar == 'A'){
		CRect rect;
		GetWindowRect(&rect);

		this->nsChart->DragFind(this->groupStructure, this->groupSelectedStructure, 
			this->selectedStructure, 0, 0, rect.Width(), rect.Height());

	}

	Long  q = 0;
	//CTRL + G �� ������ ��
	if (nChar == 'g' || nChar == 'G'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		q = this->groupStructure->Add(this->selectedStructure);
		this->selectedStructure->Clear();
		if (this->groupSelectedStructure->GetLength() > 0) {
			this->groupSelectedStructure->Clear();
		}
		this->groupSelectedStructure->Add(&this->groupStructure->GetAt(q));
		this->markStructure = 0;

	}

	//�׷������ϴ�
	if (nChar == 'u' || nChar == 'U'){
		if (this->selectedStructure->GetLength() == 0) {
			this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
			Long z = 0;
			this->groupIndexes->Clear();
			while (z < this->groupSelectedStructure->GetLength()) {
				Long index = this->groupStructure->Search(this->groupSelectedStructure->GetAt(z));
				this->groupIndexes->Add(index);
				z++;
			}
			this->groupIndexes->Arrange();
			
			z = 0;
			while (z < this->groupIndexes->GetLength()) {
				Long p = 0;
				Group group = this->groupStructure->GetAt(this->groupIndexes->GetAt(z));
				while (p < group.GetLength()) {
					this->selectedStructure->Add(group.GetAt(p));
					p++;
				}
				z++;
			}
			
			z = 0;
			while (z < this->groupIndexes->GetLength()) {
				this->groupStructure->Delete(this->groupIndexes->GetAt(z));
				z++;
			}
			this->groupSelectedStructure->Clear();
			this->Invalidate();
		}
	}

	//Delete�� ����������
	if (nChar == VK_DELETE) {
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Delete);
		keyBoard->DeleteFunction(this->selectedStructure, this->nsChart);
		keyBoard->DeleteGroupFunction(this->groupIndexes, this->groupSelectedStructure, this->groupStructure, this->nsChart);
		this->markStructure = NULL;
		this->Invalidate();
	}

	//Ctrl + r�� ���� ������ �������� �ڵ������
	if (nChar == 'r' || nChar == 'R'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		AutoCollectorGetMaxWidth *autoCollectorGetMaxWidth = new AutoCollectorGetMaxWidth;
		
		autoCollectorGetMaxWidth->GetMaxWidth(this->selectedStructure, this->nsChart);
		
		if (autoCollectorGetMaxWidth != NULL){

			delete autoCollectorGetMaxWidth;
		}
	}

	if (nChar == 'n' || nChar == 'N'){
	
		this->nsChartFormScrollBar->flyingDuckChart->NewMenu(&this->nsChartFormScrollBar->flyingDuckChart->changedStatus, &this->nsChartFormScrollBar->flyingDuckChart->fileName, this);
	}

	if (nChar == 'o' || nChar == 'O'){
	
		this->nsChartFormScrollBar->flyingDuckChart->OpenMenu(&this->nsChartFormScrollBar->flyingDuckChart->changedStatus, &this->nsChartFormScrollBar->flyingDuckChart->fileName, this);
	}

	if (nChar == 's' || nChar == 'S'){
	
		this->nsChartFormScrollBar->flyingDuckChart->SaveMenu(&this->nsChartFormScrollBar->flyingDuckChart->changedStatus, &this->nsChartFormScrollBar->flyingDuckChart->fileName, this);
	}


	//�Ҵ�� keyBoard�� �����Ѵ�
	if (keyBoard != NULL){
		delete keyBoard;
	}
}


void NSChartForm::StructureMenu(UINT nChar){
	switch (nChar) {
	case (UINT)'1':
		this->selectedStructure->Clear();
		if (this->preview != NULL){
			delete this->preview;
		}
		this->preview = new Sequence(this->currentX, this->currentY, 450, 40, "");
		break;

	case (UINT)'2':
		this->selectedStructure->Clear();
		if (this->preview != NULL){
			delete this->preview;
		}
		this->preview = new Selection(this->currentX, this->currentY, 450, 120, "", this->currentX + 225, this->currentY + 40);
		break;

	case (UINT)'3':
		this->selectedStructure->Clear();
		if (this->preview != NULL){
			delete this->preview;
		}
		this->preview = new Iteration(this->currentX, this->currentY, 450, 120, "", this->currentX + 50, this->currentY + 40);
		break;

	case (UINT)'4':
		this->selectedStructure->Clear();
		if (this->preview != NULL){
			delete this->preview;
		}
		this->preview = new Case(this->currentX, this->currentY, 450, 200, "", (this->currentX + 450) - (450 / (3 + 2)), this->currentY + (200 / 5), 3);
		break;
	}
}


void NSChartForm::OnClose() {
	
	if (this->form != NULL) {
		this->form->OnClose();
	}
	
	if (this->nsChart != NULL) {
		
		delete this->nsChart;
	}

	if (this->selectedStructure != NULL) {
		delete this->selectedStructure;
	}

	if (this->copyStructure != NULL) {
		delete this->copyStructure;
	}

	if (this->preview != NULL){
		delete this->preview;
	}

	if (this->pickStructure != NULL){
		delete this->pickStructure;
	}

	if (this->groupStructure != NULL) {
		delete this->groupStructure;
	}
	
	if (this->groupSelectedStructure != NULL) {
		delete this->groupSelectedStructure;
	}

	if (this->groupCopyStructure != NULL) {
		delete this->groupCopyStructure;
	}

	if (this->groupIndexes != NULL) {
		delete this->groupIndexes;
	}

	if (this->editCopy != NULL){
		delete[] this->editCopy;
	}

	CWnd::DestroyWindow();

}


Long NSChartForm::Load(CString str) {
	char line[256];
	Structure *structure;
	StructureFactory structureFactory;
	Long i;
	Long parentCode;
	Long typeCode;
	Long count = 0;
	Long *parentCodes = new Long[100];
	Structure* (*parentStructures) = new Structure*[100];
	ifstream file;
	
	file.open(str, ios::in);
	file.getline(line, 256);
	
	while (!file.eof()) {
		
		sscanf(line, "%d:%d", &parentCode, &typeCode);
		structure = structureFactory.CreateStructure(line);
		
		if (parentCode == 100) {
			//�θ� �������ش�.
			structure->SetParent(this->nsChart);
			/////////////////////////////////////
			this->nsChart->Add(structure);
			
			if (count != 0) {
				i = 0;
				
				while (i < count) {
					parentStructures[i] = 0;
					parentCodes[i] = 0;
					i++;
				}
				count = 0;
			}
			i = 0;
			parentStructures[i] = structure;
			parentCodes[i] = parentCode;
			count++;
		}
		else {
			i = 0;
			
			while (parentCodes[i] != parentCode - 1) {
				i++;
			}
			//Block�� �θ� �������ش�.
			structure->SetParent(parentStructures[i]);
			//////////////////////////////////////////
			parentStructures[i]->Add(structure);
			
			if (typeCode != 1) {
				parentStructures[i + 1] = structure;
				parentCodes[i + 1] = parentCode;
				count++;
			}
		}
		file.getline(line, 256);
	}

	if (parentCodes != 0) {
		delete[] parentCodes;
	}

	if (parentStructures != 0) {
		delete[] parentStructures;
	}

	file.close();
	
	return this->nsChart->GetLength();
}


Long NSChartForm::Save(CString str) {
	Structure *structure;
	Long i = 0;
	Long count = 0;
	Long index = 100;
	Visitor *visitor;
	visitor = new Saver;
	NSChart temp;
	Long(*integers);
	integers = new Long[100];
	CString strr;
	strr.Format("%s", str);

	//remove("Structures.txt");
	ofstream file(strr);
	
	
	if (this->nsChart != 0) {
		if (file.is_open()) {
			while (i < this->nsChart->GetLength() && !file.eof()) {
				structure = this->nsChart->GetChild(i);
				structure->Accept(visitor, &temp, integers, &index, &count);
				i++;
			}
			i = 0;
			while (i < count) {
				file << integers[i] << ":" << temp.GetChild(i)->GetString() << endl;
				i++;
			}
		}
		if (visitor != NULL) {
			delete visitor;
		}
		if (integers != NULL) {
			delete integers;
		}

		file.close();
	}
	return this->nsChart->GetLength();
}

