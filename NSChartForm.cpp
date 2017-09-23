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
	DrawImage(&memDC);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//사용된 메모리 DC 및 BITMAP의 삭제
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

	//기호를 투명하게 한다.
	pDC->SelectStockObject(NULL_BRUSH);

	//마우스에 기호붙이기
	if (this->preview != NULL){
		this->previewMove = true;
		this->preview->Accept(visitor, pDC);
	}

	//드래그를 하면
	if (this->drag == true){
		this->rect.m_rect = CRect(this->previousX, this->previousY, this->currentX, this->currentY);
		this->rect.m_nStyle = CRectTracker::dottedLine;
		this->rect.Draw(pDC);
	}
	
	i = 0;
	//기호 그리기
	while (i < this->nsChart->GetLength()) {
		structure = this->nsChart->GetChild(i);
		structure->Accept(visitor, pDC);
		i++;
	}

	//빼기할 때 뺴기된 도형을 보여준다.
	if (this->pickStructure != NULL){
		this->pickStructure->Accept(visitor, pDC);
	}

	//빼기할 때 빼기된 그룹의 도형을 보여준다.
	if (this->pickGroupStructure != NULL) {
		Long j = 0;
		while (j < this->pickGroupStructure->GetLength()) {
			this->pickGroupStructure->GetAt(j)->Accept(visitor, pDC);
			j++;
		}
	}

	//다중선택 보여주기
	SelectedDrawer *selectedDrawer = new SelectedDrawer;
	selectedDrawer->DrawStructure(this->selectedStructure, this->rect, pDC);

	//그룹스트럭쳐 보여주기
	selectedDrawer->DrawGroupStructure(this->groupSelectedStructure, this->rect, pDC);

	//삽입할 위치를 보여주는 작업
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
		
		//기호를 투명하게 한다.
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
	MouseLButton *mouseLButton = new MouseLButton(new CtrlSelectStructure); //컨트롤 누른 선택상태를 디폴트값으로
	CRectTracker rect;
	Long i = 0;
	Structure *inGroupStructure = 0;
	this->result = -1;
	
	//1.1 현재 좌표를 구한다.
	this->currentX = point.x;
	this->currentY = point.y;
	this->previousX = this->currentX;
	this->previousY = this->currentY;

	//1.2 기호를 찾는다.
	structure = this->nsChart->Find(this->currentX, this->currentY);
	
	//컨트롤 선택
	if (nFlags & MK_CONTROL && this->preview == NULL) {
		
		//그룹 컨트롤 선택인지 
		mouseLButton->SelectGroup(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, structure, &inGroupStructure);

		//찾은 structure가 그룹에 없으면 그냥 컨트롤 선택
		if (inGroupStructure == 0) {
			mouseLButton->Select(this->selectedStructure, structure, point.x, point.y, &result);
		}
	}
	//그냥 선택
	else if (this->preview == NULL) {
		
		mouseLButton->SetState(new JustSelectStructure);
		//그룹 선택인지
		mouseLButton->SelectGroup(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, structure, &inGroupStructure);
		
		//찾은 structure가 그룹에 없으면 그냥 선택
		if (inGroupStructure == 0) {
			mouseLButton->Select(this->selectedStructure, structure, point.x, point.y, &result);
		}
		
	}
	
	//그룹이 선택되어 있다면,
	if (this->groupSelectedStructure->GetLength() > 0) {
		this->groupStructureMove = true;
	}
		
	if (mouseLButton != NULL) {
		delete mouseLButton;
	}

	//초기화한다
	this->Invalidate();

	if (structure != 0 && this->preview == NULL) {
	rect.m_rect = CRect(structure->GetX(), structure->GetY(), structure->GetX() + structure->GetWidth(), structure->GetY() + structure->GetHeight());
	//도형을 움직일 수 있게 시작좌표를 지정한다
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

	//삽입할 위치를 찾는다.
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

	//마우스를 움직일때 도형도 움직인다
	if (this->editOpen == false && this->startToMove == true) {

		//빼다
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
		//그룹스트럭쳐를 빼다
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

	//마우스를 움직일때 새로생성하는 preview Structure가 따라다님
	else if (this->previewMove == true && this->preview != NULL){
		this->preview->Correct(this->currentX - this->preview->GetX(), this->currentY - this->preview->GetY());
		this->Invalidate();
	}
	
	//도형 크기 조절을 할때
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
		//크기 조절했을때 밑에 도형 자동으로 내려감  비지터 만들어야함
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
		//오른쪽 아래에서 왼쪽 위로 드래그 할 때
		if (this->previousX > this->currentX && this->previousY > this->currentY){
			temp = this->previousX;
			this->previousX = this->currentX;
			this->currentX = temp;

			temp = this->previousY;
			this->previousY = this->currentY;
			this->currentY = temp;
		}
		//오른쪽 위에서 왼쪽 아래로 드래그 할 때
		else if (this->previousX > this->currentX && this->previousY < this->currentY){
			temp = this->previousX;
			this->previousX = this->currentX;
			this->currentX = temp;

		}
		//왼쪽 아래에서 오른쪽 위로 드래그 할 때
		else if (this->previousX < this->currentX && this->previousY > this->currentY){
			temp = this->previousY;
			this->previousY = this->currentY;
			this->currentY = temp;
		}
		this->nsChart->DragFind(this->groupStructure, this->groupSelectedStructure, this->selectedStructure, this->previousX, this->previousY, this->currentX, this->currentY);
	}
	this->drag = false;


	//기호가 움직였으면
	if (this->startToMove == true || this->previewMove == true || this->groupStructureMove == true) {
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		//1.1 기호를 삽입할 때
		
		//기존에 있는 기호를 넣을 때
		if (this->pickStructure != NULL){

			insertStructure = this->pickStructure;
		}
		//새로운 기호를 넣을 때
		else if (this->preview != NULL){
			
			insertStructure = this->preview;

		}
		//그룹을 넣을 때
		if (this->pickGroupStructure != NULL) {

			insertGroup = this->pickGroupStructure;
		}

		// NSChart에 삽입할 때,
		if (this->markStructure != NULL){

			mouseLButton = new MouseLButton(new InsertStructure);
			
			//기호만 삽입할 때,
			if (insertStructure != NULL && insertGroup == NULL) {

				mouseLButton->Insert(insertStructure, this->markStructure, this->checkingResult);

			}
			
			//그룹만 삽입할 때,
			if (insertGroup != NULL && insertStructure == NULL) {

				mouseLButton->InsertGroup(insertGroup, this->markStructure, this->checkingResult);

			}

			//기호와 그룹을 동시에 삽입할 때,
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

		// 빈 종이에 삽입할 때,
		else if(this->markStructure == NULL){
			
			//기호를 삽입할 때,
			if (insertStructure != NULL && insertGroup == NULL) {
				insertStructure->SetParent(this->nsChart);
				this->nsChart->Add(insertStructure);
			}

			//그룹을 삽입할 때,
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

			//기호와 그룹을 동시에 삽입할 때,
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
				
		//마지막 위치를 지정해준다.
		this->endPos = point;

		this->startToMove = false;
		this->pickStructure = NULL;
		this->preview = NULL;
		this->previewMove = false;
		this->groupStructureMove = false;
		this->pickGroupStructure = NULL;

	}
		
	this->nsChart->Arrange();

	//기호 크기를 조절했으면
	if (this->startToResize == true){

		this->endPos = point;
		this->startToResize = false;

	}

	if (mouseLButton != NULL){
		delete mouseLButton;
	}

	this->Invalidate();
	//손을 마우스에서 뗀다
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
						//Drawer.cpp와 연동해서 그려준다.
						CRect(this->selectedStructure->GetAt(0)->GetX() + (divideWidth * j) + (divideWidth / 2),
							dynamic_cast<Case *>(structure)->GetMidY() + (midY2 - dynamic_cast<Case *>(structure)->GetMidY()) / 2,
							this->selectedStructure->GetAt(0)->GetX() + (divideWidth * (j + 1)), 
							midY2), this, 10000);
					//CFrameWnd에 있는 내용을 지운다.
					dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->CaseStringModify(j, "");
					this->form->SetFocus();
				}
			}
			else {
				this->form = new Edit;
				this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE, 
					//Drawer.cpp와 연동해서 그려준다.
					CRect(dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetX() + (dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetWidth() / 2),
						(dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetY() + (dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Case*>(this->selectedStructure->GetAt(0))->GetY()) / 5),
						this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(), 
						this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1/ 2), this, 10000);
				//CFrameWnd에 있는 내용을 지운다.
				this->selectedStructure->GetAt(0)->CorrectContents("");
				this->form->SetFocus();
			}

		}
		else if (dynamic_cast<Iteration*>(structure)){
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE,
				//Drawer.cpp와 연동해서 그려준다.
				CRect(dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidX() - (dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidX() - dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetX()) / 2,
					dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidY() - (dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Iteration*>(this->selectedStructure->GetAt(0))->GetY()) * 2 / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(),
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1 / 2), this, 10000);
			//CFrameWnd에 있는 내용을 지운다.
			this->selectedStructure->GetAt(0)->CorrectContents("");
			this->form->SetFocus();
		}
		
		else if (dynamic_cast<Selection*>(structure)){
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE,
				//Drawer.cpp와 연동해서 그려준다.
				CRect(dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidX() - (dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetWidth() / 30) - 55,
					dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidY() - (dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetMidY() - dynamic_cast<Selection*>(this->selectedStructure->GetAt(0))->GetY()) * 2 / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(),
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() * 1 / 2), this, 10000);
			//CFrameWnd에 있는 내용을 지운다.
			this->selectedStructure->GetAt(0)->CorrectContents("");
			this->form->SetFocus();
		}

		else {
			this->form = new Edit;
			this->form->Create(NULL, TEXT("Editor"), WS_CHILD | WS_VISIBLE, 
				//Drawer.cpp와 연동해서 그려준다.
				CRect(this->selectedStructure->GetAt(0)->GetX() + 25,
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight() / 3,
					this->selectedStructure->GetAt(0)->GetX() + this->selectedStructure->GetAt(0)->GetWidth(), 
					this->selectedStructure->GetAt(0)->GetY() + this->selectedStructure->GetAt(0)->GetHeight()), this, 10000);
			//CFrameWnd에 있는 내용을 지운다.
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

	//메뉴에있는 모든 키다운기능
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

	//ESC버튼 눌렀을때
	if (nChar == VK_ESCAPE){

		keyBoard->Clear(this->copyStructure);

		if (this->preview != NULL){
			delete this->preview;
			this->preview = NULL;
			this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 0;
		}
	}

	//Ctrl + p가 눌러졌을때 스위치 케이스문에 더해준다.
	if (nChar == 'p' || nChar == 'P') {
		if (IsCTRLPressed() == TRUE) {
			
			if (this->selectedStructure->GetLength() > 0) {
				structure = this->selectedStructure->GetAt(0);
				if (dynamic_cast<Case *>(structure)) {
					this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
					Long count = dynamic_cast<Case *>(structure)->GetCount();
					count++;
					dynamic_cast<Case *>(structure)->CorrectCaseCount(count);
					//자식을 빼준다.
					Long z = 0;
					while (z != dynamic_cast<Case *>(structure)->GetLength()) {
						dynamic_cast<Case *>(structure)->GetChild(z)->Accept(visitor, this->nsChart, structure);
						
					}
				}
				this->Invalidate();
			}
		}
	}

	//Ctrl + m이 눌러졌을 때 스위치 케이스문에 빼준다.
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
					//자식을 빼준다.
					Long z = 0;
					while (z != dynamic_cast<Case *>(structure)->GetLength()) {
						dynamic_cast<Case *>(structure)->GetChild(z)->Accept(visitor, this->nsChart, structure);
					}
				}
				this->Invalidate();
			}
		}
	}

	//할당된 keyBoard를 해제한다
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

	//Ctrl + x가 눌러졌을때
	if (nChar == 'x' || nChar == 'X'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Cut);
		keyBoard->CutFunction(this->selectedStructure, this->copyStructure, this->nsChart);
		keyBoard->CutGroupFunction(this->groupSelectedStructure, this->groupCopyStructure, this->nsChart);
		this->Invalidate();
		this->ClipBoardCopy();
	}

	//CTRL + C 를 눌렀을때
	if (nChar == 'c' || nChar == 'C'){
		
		keyBoard->SetKeyBoardAction(new Copy);
		keyBoard->CopyFunction(this->selectedStructure, this->copyStructure);
		keyBoard->CopyGroupFunction(this->groupSelectedStructure, this->groupCopyStructure);
		this->ClipBoardCopy();
	}

	//CTRL + V를 눌렀을때
	if (nChar == 'v' || nChar == 'V'){
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Paste);
		//keyBoard->PasteFunction(this->copyStructure, this->currentX, this->currentY, this->nsChart);
		//keyBoard->PasteGroupFunction(this->groupCopyStructure, this->groupStructure, this->currentX, this->currentY, this->nsChart);
		keyBoard->CombinedPasteFunction(this->copyStructure, this->groupCopyStructure, this->groupStructure, this->currentX, this->currentY, this->nsChart);
		this->Invalidate();
				
	}

	//전 기호를 선택한다.
	if (nChar == 'a' || nChar == 'A'){
		CRect rect;
		GetWindowRect(&rect);

		this->nsChart->DragFind(this->groupStructure, this->groupSelectedStructure, 
			this->selectedStructure, 0, 0, rect.Width(), rect.Height());

	}

	Long  q = 0;
	//CTRL + G 를 눌렀을 때
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

	//그룹해제하다
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

	//Delete가 눌러졌을때
	if (nChar == VK_DELETE) {
		this->nsChartFormScrollBar->flyingDuckChart->changedStatus = 1;
		keyBoard->SetKeyBoardAction(new Delete);
		keyBoard->DeleteFunction(this->selectedStructure, this->nsChart);
		keyBoard->DeleteGroupFunction(this->groupIndexes, this->groupSelectedStructure, this->groupStructure, this->nsChart);
		this->markStructure = NULL;
		this->Invalidate();
	}

	//Ctrl + r이 눌러 졌을때 도형들을 자동맞춘다
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


	//할당된 keyBoard를 해제한다
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
			//부모를 지정해준다.
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
			//Block의 부모를 지정해준다.
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

