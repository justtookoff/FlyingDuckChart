//FlyingDuckChart.cpp
#include "FlyingDuckChart.h"
#include "NSChartFormScrollBar.h"
#include "NSChartForm.h"
#include "AutoCollectorGetMaxWidth.h"
#include "Toolbar.h"
#include "SelectedStructure.h"
#include "GroupSelectedStructure.h"
#include "HelpDialog.h"
#include "About.h"
#include "resource.h"
#include "Edit.h"
#include "PrintPreview.h"
#include "PrintDrawer.h"
#include "NSChart.h"
#include "Visitor.h"

#include <commdlg.h>

#pragma warning(disable:4996)
#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))
#define IsSHIFTPressed() ( 0x8000 ==(GetKeyState(VK_SHIFT) & 0x8000 ))

BEGIN_MESSAGE_MAP(FlyingDuckChart, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//파일
	ON_COMMAND(ID_NEW,OnNewMenuClicked)
	ON_COMMAND(ID_OPEN,OnOpenMenuClicked)
	ON_COMMAND(ID_SAVE,OnSaveMenuClicked)
	ON_COMMAND(ID_SAVE_AS,OnAnotherSaveMenuClicked)
	ON_COMMAND(ID_PRINT, OnPrintMenuClicked)
	ON_COMMAND(ID_PRINTPREVIEW, OnPrintPreviewMenuClicked)
	ON_COMMAND(ID_EXIT,OnExitMenuClicked)
	//편집
	ON_COMMAND(ID_CUT,OnCutMenuClicked)
	ON_COMMAND(ID_COPY,OnCopyMenuClicked)
	ON_COMMAND(ID_PASTE,OnPasteMenuClicked)
	ON_COMMAND(ID_DELETE,OnDeleteMenuClicked)
	ON_COMMAND(ID_SELECTALL,OnSelectAllMenuClicked)
	ON_COMMAND(ID_GROUPING,OnGroupMenuClicked)
	ON_COMMAND(ID_UNLOCKGROUPING,OnUnLockGroupMenuClicked)
	ON_COMMAND(ID_AUTOCORRECT,OnAutoCorrectMenuClicked)
	//기호
	ON_COMMAND(ID_SEQUENCE,OnSequenceMenuClicked)
	ON_COMMAND(ID_SELECTION,OnSelectionMenuClicked)
	ON_COMMAND(ID_ITERATION,OnIterationMenuClicked)
	ON_COMMAND(ID_CASE,OnCaseMenuClicked)
	//도움말
	ON_COMMAND(ID_HELP_HELP,OnHelpMenuClicked)
	ON_COMMAND(ID_INFO, OnInfoMenuClicked)
	//우클릭
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_CUT, OnPopupCutMenuClicked)
	ON_COMMAND(ID_POPUP_COPY, OnPopupCopyMenuClicked)
	ON_COMMAND(ID_POPUP_PASTE, OnPopupPasteMenuClicked)
	ON_COMMAND(ID_POPUP_DELETE, OnPopupDeleteMenuClicked)
	ON_COMMAND(ID_POPUP_SELECTALL, OnPopupSelectAllMenuClicked)
	ON_COMMAND(ID_POPUP_GROUPING, OnPopupGroupMenuClicked)
	ON_COMMAND(ID_POPUP_UNLOCKGROUPING, OnPopupUnLockGroupMenuClicked)
	ON_COMMAND(ID_POPUP_AUTOCORRECT, OnPopupAutoCorrectMenuClicked)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FlyingDuckChart::FlyingDuckChart() {
	this->nsChartFormScrollBar = NULL;
	this->toolbar = NULL;
	this->printPreview = NULL;
}

FlyingDuckChart::~FlyingDuckChart(){

}

int FlyingDuckChart::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	////// 아이콘 등록 ////////
	HICON hLargeIcon = AfxGetApp()->LoadIcon(IDI_ICON_DUCK); // 32*32 아이콘 선택
	HICON hSmallIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_DUCK), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	SetIcon(hSmallIcon, TRUE);
	////// 아이콘 등록 ////////
	this->fileName = "";
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	this->changedStatus = 0;

	CRect rect;
	GetWindowRect(&rect);

	//툴바윈도우

	this->toolbar = new Toolbar;
	this->toolbar->Create(NULL, TEXT("Toolbar"), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 75, rect.Height() * 5), this, 10001);

	this->nsChartFormScrollBar = new NSChartFormScrollBar;
	this->nsChartFormScrollBar->Create(NULL, TEXT("NSChartFormScrollBar"), WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		CRect(75, 0, rect.Width() + rect.Width() /3 - rect.Width() /120, rect.Height() + rect.Height() / 4 + rect.Height() / 20), this, 10002);
	this->nsChartFormScrollBar->nsChartForm->SetFocus();

	return 0;
}

void FlyingDuckChart::OnPaint(){
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);



	ReleaseDC(pDC);

	EndPaint(&Ps);
}

//파일
void FlyingDuckChart::OnNewMenuClicked(){

	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.변경내용이 있을때
	if (this->changedStatus > 0){

		//1.1.1.메세지 박스를 띄운다
		result = MessageBox("변경 사항을 저장하시겠습니까?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.메세지박스에서 저장하기를 눌렀을때
		if (result == IDYES){

			//1.1.2.1.파일이 기존파일일때
			if (this->fileName != ""){
				//1.1.2.1.1.기존파일에 저장한다
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.파일이 새파일일때
			else{

				//1.1.2.2.1.다른이름으로 저장하기 창을 띄운다
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
					// 여기서 파일 저장 작업.
					// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

					this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetPathName());
				}
			}
		}
	}
	//1.2.메세지박스 결과가 취소가 아닐경우
	if (result != IDCANCEL){

		//1.2.1.현재창의 모든 도형을 지운다
		this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
		this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
		this->changedStatus = 0;
		this->fileName = "";
		this->Invalidate();
	}

	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnOpenMenuClicked(){

	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.변경내용이 있을때
	if (this->changedStatus > 0){

		//1.1.1.메세지 박스를 띄운다
		result = MessageBox("변경 사항을 저장하시겠습니까?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.메세지박스에서 저장하기를 눌렀을때
		if (result == IDYES){

			//1.1.2.1.파일이 기존파일일때
			if (this->fileName != ""){
				//1.1.2.1.1.기존파일에 저장한다
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.파일이 새파일일때
			else{

				//1.1.2.2.1.다른이름으로 저장하기 창을 띄운다
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
					// 여기서 파일 저장 작업.
					// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

					this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetPathName());
				}
			}
		}
	}

	if (result != IDCANCEL){


		CFileDialog dlg(TRUE, "txt", NULL,
			OFN_HIDEREADONLY,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){
			//1.1.2.3.현재창의 모든 도형을 지운다
			this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
			this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
			//1.1.2.4. 로드한다
			this->nsChartFormScrollBar->nsChartForm->Load(dlg.GetPathName());
			this->fileName = dlg.GetPathName();
			this->changedStatus = 0;
			this->Invalidate();

		}
	}
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnSaveMenuClicked(){

	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	if (this->fileName == ""){

		CFileDialog dlg(false, "txt", NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
			// 여기서 파일 저장 작업.
			// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

			this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetPathName());
			this->fileName = dlg.GetPathName();
			this->changedStatus = 0;
		}

	}
	else{

		this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
		this->changedStatus = 0;
	}

	this->nsChartFormScrollBar->nsChartForm->SetFocus();

}

void FlyingDuckChart::OnAnotherSaveMenuClicked(){

	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	//다이얼로그 호출
	CFileDialog dlg(false, "txt", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Text File(*.txt)|*.txt||");

	if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
		// 여기서 파일 저장 작업.
		// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

		this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetPathName());
		this->changedStatus = 0;
	}

	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnPrintPreviewMenuClicked(){
	CRect rect;
	GetWindowRect(&rect);

	this->printPreview = new PrintPreview;
	this->printPreview->Create(NULL, TEXT("PrintPreview"), WS_POPUP | WS_VISIBLE | WS_MAXIMIZE,
		CRect(0, 0, rect.Width() - 20, rect.Height() + 20), this);
	this->printPreview->SetFocus();
	
}

void FlyingDuckChart::OnPrintMenuClicked(){
	this->Print();
}

void FlyingDuckChart::Print(){
	Visitor *visitor = NULL;
	visitor = new PrintDrawer;

	PRINTDLG pd;
	DEVNAMES *pDevNames;
	DEVMODE *pDevMode;
	

	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));

	/*wchar_t *wt;
	char *c;*/

	pd.lStructSize = sizeof(pd);
	pd.hwndOwner = NULL;
	pd.hDevMode = NULL;     // Don't forget to free or store hDevMode.
	pd.hDevNames = NULL;     // Don't forget to free or store hDevNames.
	pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	pd.nCopies = 1;
	/*pd.nFromPage = 0xFFFF;
	pd.nToPage = 0xFFFF;
	pd.nMinPage = 1;
	pd.nMaxPage = 0xFFFF;*/

	if (PrintDlg(&pd) == TRUE){

		// GDI calls to render output. 
		pDevMode = (DEVMODE*)pd.hDevMode;
		pDevNames = (DEVNAMES*)pd.hDevNames;

	/*	LPCTSTR     szDriver;
		LPCTSTR     szDevice;
		LPCTSTR     szOutput;
		szDriver = (LPCTSTR)pDevNames + pDevNames->wDriverOffset;
		szDevice = (LPCTSTR)pDevNames + pDevNames->wDeviceOffset;
		szOutput = (LPCTSTR)pDevNames + pDevNames->wOutputOffset;

		wprintf(L"%s", szDevice);*/
	}

}

void FlyingDuckChart::OnExitMenuClicked(){

	this->OnClose();
}

//편집
void FlyingDuckChart::OnCutMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('X');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnCopyMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('C');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPasteMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('V');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnDeleteMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnSelectAllMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnGroupMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('G');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnUnLockGroupMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('U');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnAutoCorrectMenuClicked(){


	AutoCollectorGetMaxWidth *autoCollectorGetMaxWidth = new AutoCollectorGetMaxWidth;

	autoCollectorGetMaxWidth->GetMaxWidth(this->nsChartFormScrollBar->nsChartForm->selectedStructure, this->nsChartFormScrollBar->nsChartForm->nsChart);

	if (autoCollectorGetMaxWidth != NULL){

		delete autoCollectorGetMaxWidth;
	}
	this->nsChartFormScrollBar->nsChartForm->SetFocus();

}
//기호
void FlyingDuckChart::OnSequenceMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->StructureMenu('1');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnSelectionMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->StructureMenu('2');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnIterationMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->StructureMenu('3');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnCaseMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->StructureMenu('4');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

//도움말
void FlyingDuckChart::OnHelpMenuClicked(){

	HelpDialog helpDialog;
	helpDialog.DoModal();
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

//About
void FlyingDuckChart::OnInfoMenuClicked(){
	About about;
	about.DoModal();
	this->nsChartFormScrollBar->nsChartForm->SetFocus();

}

//우클릭
void FlyingDuckChart::OnContextMenu(CWnd *pWnd, CPoint point){
	CMenu menu;

	// 리소스에 설정한 메뉴정보를 읽어온다.
	menu.LoadMenu(IDR_MENU2);

	// 하위 메뉴의 첫번째 정보를 얻는다.
	CMenu *p_sub_menu = menu.GetSubMenu(0);

	CPoint pos;
	GetCursorPos(&pos);

	// 마우스 커서가 위치한 곳에 팝업메뉴를 출력한다.
	p_sub_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);

	// 생성된 팝업 메뉴를 삭제한다.
	menu.DestroyMenu();
}

void FlyingDuckChart::OnPopupCutMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('X');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPopupCopyMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('C');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPopupPasteMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('V');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPopupDeleteMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPopupSelectAllMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}
void FlyingDuckChart::OnPopupGroupMenuClicked(){
	if (this->nsChartFormScrollBar->nsChartForm->selectedStructure->GetLength() > 0){
		this->nsChartFormScrollBar->nsChartForm->EditMenu('G');
		this->nsChartFormScrollBar->nsChartForm->SetFocus();
	}
}
void FlyingDuckChart::OnPopupUnLockGroupMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('U');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnPopupAutoCorrectMenuClicked(){
	this->nsChartFormScrollBar->nsChartForm->EditMenu('R');
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}


BOOL FlyingDuckChart::OnEraseBkgnd(CDC *pDC){
	return TRUE;
}

void FlyingDuckChart::OnClose(){

	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	Long result = 0;
	//1.1.변경내용이 있을때
	if (this->changedStatus > 0){

		//1.1.1.메세지 박스를 띄운다
		result = MessageBox("변경 사항을 저장하시겠습니까?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.메세지박스에서 저장하기를 눌렀을때
		if (result == IDYES){

			//1.1.2.1.파일이 기존파일일때
			if (this->fileName != ""){
				//1.1.2.1.1.기존파일에 저장한다
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.파일이 새파일일때
			else{

				//1.1.2.2.1.다른이름으로 저장하기 창을 띄운다
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ 

					this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetFileName());
				}
			}
		}
	}

	//1.2.메세지박스의 결과가 취소버튼이 아닌경우
	if (result != IDCANCEL){
		//1.2.1.끝낸다
		
		if (this->nsChartFormScrollBar != NULL){
			delete this->nsChartFormScrollBar;
		
		}
		if (this->toolbar != NULL) {
			delete this->toolbar;
		}

		CFrameWnd::OnClose();

	}
	else{
		this->nsChartFormScrollBar->nsChartForm->SetFocus();
	}


}


void FlyingDuckChart::NewMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2){
	
	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.변경내용이 있을때
	if (*changedStatus2 > 0){

		//1.1.1.메세지 박스를 띄운다
		result = MessageBox("변경 사항을 저장하시겠습니까?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.메세지박스에서 저장하기를 눌렀을때
		if (result == IDYES){

			//1.1.2.1.파일이 기존파일일때
			if (*fileName2 != ""){
				//1.1.2.1.1.기존파일에 저장한다
				nsChartForm2->Save(*fileName2);
			}
			//1.1.2.2.파일이 새파일일때
			else{

				//1.1.2.2.1.다른이름으로 저장하기 창을 띄운다
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
					// 여기서 파일 저장 작업.
					// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

					nsChartForm2->Save(dlg.GetPathName());
				}
			}
		}
	}
	//1.2.메세지박스 결과가 취소가 아닐경우
	if (result != IDCANCEL){

		//1.2.1.현재창의 모든 도형을 지운다
		nsChartForm2->EditMenu('A');
		nsChartForm2->EditMenu(VK_DELETE);
		*changedStatus2 = 0;
		*fileName2 = "";
		this->Invalidate();
	}

	nsChartForm2->SetFocus();
}

void FlyingDuckChart::OpenMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2){
	
	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}
	
	int result = 0;
	//1.1.변경내용이 있을때
	if (*changedStatus2 > 0){

		//1.1.1.메세지 박스를 띄운다
		result = MessageBox("변경 사항을 저장하시겠습니까?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.메세지박스에서 저장하기를 눌렀을때
		if (result == IDYES){

			//1.1.2.1.파일이 기존파일일때
			if (*fileName2 != ""){
				//1.1.2.1.1.기존파일에 저장한다
				nsChartForm2->Save(*fileName);
			}
			//1.1.2.2.파일이 새파일일때
			else{

				//1.1.2.2.1.다른이름으로 저장하기 창을 띄운다
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
					// 여기서 파일 저장 작업.
					// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

					nsChartForm2->Save(dlg.GetPathName());
				}
			}
		}
	}

	if (result != IDCANCEL){


		CFileDialog dlg(TRUE, "txt", NULL,
			OFN_HIDEREADONLY,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){
			//1.1.2.3.현재창의 모든 도형을 지운다
			nsChartForm2->EditMenu('A');
			nsChartForm2->EditMenu(VK_DELETE);
			//1.1.2.4. 로드한다
			nsChartForm2->Load(dlg.GetPathName());
			*fileName2 = dlg.GetPathName();
			*changedStatus2 = 0;
			this->Invalidate();

		}
	}
	nsChartForm2->SetFocus();
}


void FlyingDuckChart::SaveMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2){
	
	//edit창이 열려있으면 강제로 끝낸다.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	if (*fileName2 == ""){

		CFileDialog dlg(false, "txt", NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){ // OK 를 하는 순간 현재 경로가 변경된다.
			// 여기서 파일 저장 작업.
			// 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

			nsChartForm2->Save(dlg.GetPathName());
			*fileName2 = dlg.GetPathName();
			*changedStatus2 = 0;
		}

	}
	else{

		nsChartForm2->Save(*fileName2);
		*changedStatus2 = 0;
	}
	nsChartForm2->SetFocus();
}

