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
	//����
	ON_COMMAND(ID_NEW,OnNewMenuClicked)
	ON_COMMAND(ID_OPEN,OnOpenMenuClicked)
	ON_COMMAND(ID_SAVE,OnSaveMenuClicked)
	ON_COMMAND(ID_SAVE_AS,OnAnotherSaveMenuClicked)
	ON_COMMAND(ID_PRINT, OnPrintMenuClicked)
	ON_COMMAND(ID_PRINTPREVIEW, OnPrintPreviewMenuClicked)
	ON_COMMAND(ID_EXIT,OnExitMenuClicked)
	//����
	ON_COMMAND(ID_CUT,OnCutMenuClicked)
	ON_COMMAND(ID_COPY,OnCopyMenuClicked)
	ON_COMMAND(ID_PASTE,OnPasteMenuClicked)
	ON_COMMAND(ID_DELETE,OnDeleteMenuClicked)
	ON_COMMAND(ID_SELECTALL,OnSelectAllMenuClicked)
	ON_COMMAND(ID_GROUPING,OnGroupMenuClicked)
	ON_COMMAND(ID_UNLOCKGROUPING,OnUnLockGroupMenuClicked)
	ON_COMMAND(ID_AUTOCORRECT,OnAutoCorrectMenuClicked)
	//��ȣ
	ON_COMMAND(ID_SEQUENCE,OnSequenceMenuClicked)
	ON_COMMAND(ID_SELECTION,OnSelectionMenuClicked)
	ON_COMMAND(ID_ITERATION,OnIterationMenuClicked)
	ON_COMMAND(ID_CASE,OnCaseMenuClicked)
	//����
	ON_COMMAND(ID_HELP_HELP,OnHelpMenuClicked)
	ON_COMMAND(ID_INFO, OnInfoMenuClicked)
	//��Ŭ��
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
	////// ������ ��� ////////
	HICON hLargeIcon = AfxGetApp()->LoadIcon(IDI_ICON_DUCK); // 32*32 ������ ����
	HICON hSmallIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_DUCK), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	SetIcon(hSmallIcon, TRUE);
	////// ������ ��� ////////
	this->fileName = "";
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	this->changedStatus = 0;

	CRect rect;
	GetWindowRect(&rect);

	//����������

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

//����
void FlyingDuckChart::OnNewMenuClicked(){

	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.���泻���� ������
	if (this->changedStatus > 0){

		//1.1.1.�޼��� �ڽ��� ����
		result = MessageBox("���� ������ �����Ͻðڽ��ϱ�?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.�޼����ڽ����� �����ϱ⸦ ��������
		if (result == IDYES){

			//1.1.2.1.������ ���������϶�
			if (this->fileName != ""){
				//1.1.2.1.1.�������Ͽ� �����Ѵ�
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.������ �������϶�
			else{

				//1.1.2.2.1.�ٸ��̸����� �����ϱ� â�� ����
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
					// ���⼭ ���� ���� �۾�.
					// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

					this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetPathName());
				}
			}
		}
	}
	//1.2.�޼����ڽ� ����� ��Ұ� �ƴҰ��
	if (result != IDCANCEL){

		//1.2.1.����â�� ��� ������ �����
		this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
		this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
		this->changedStatus = 0;
		this->fileName = "";
		this->Invalidate();
	}

	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnOpenMenuClicked(){

	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.���泻���� ������
	if (this->changedStatus > 0){

		//1.1.1.�޼��� �ڽ��� ����
		result = MessageBox("���� ������ �����Ͻðڽ��ϱ�?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.�޼����ڽ����� �����ϱ⸦ ��������
		if (result == IDYES){

			//1.1.2.1.������ ���������϶�
			if (this->fileName != ""){
				//1.1.2.1.1.�������Ͽ� �����Ѵ�
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.������ �������϶�
			else{

				//1.1.2.2.1.�ٸ��̸����� �����ϱ� â�� ����
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
					// ���⼭ ���� ���� �۾�.
					// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

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
			//1.1.2.3.����â�� ��� ������ �����
			this->nsChartFormScrollBar->nsChartForm->EditMenu('A');
			this->nsChartFormScrollBar->nsChartForm->EditMenu(VK_DELETE);
			//1.1.2.4. �ε��Ѵ�
			this->nsChartFormScrollBar->nsChartForm->Load(dlg.GetPathName());
			this->fileName = dlg.GetPathName();
			this->changedStatus = 0;
			this->Invalidate();

		}
	}
	this->nsChartFormScrollBar->nsChartForm->SetFocus();
}

void FlyingDuckChart::OnSaveMenuClicked(){

	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	if (this->fileName == ""){

		CFileDialog dlg(false, "txt", NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
			// ���⼭ ���� ���� �۾�.
			// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

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

	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	//���̾�α� ȣ��
	CFileDialog dlg(false, "txt", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Text File(*.txt)|*.txt||");

	if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
		// ���⼭ ���� ���� �۾�.
		// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

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

//����
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
//��ȣ
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

//����
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

//��Ŭ��
void FlyingDuckChart::OnContextMenu(CWnd *pWnd, CPoint point){
	CMenu menu;

	// ���ҽ��� ������ �޴������� �о�´�.
	menu.LoadMenu(IDR_MENU2);

	// ���� �޴��� ù��° ������ ��´�.
	CMenu *p_sub_menu = menu.GetSubMenu(0);

	CPoint pos;
	GetCursorPos(&pos);

	// ���콺 Ŀ���� ��ġ�� ���� �˾��޴��� ����Ѵ�.
	p_sub_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);

	// ������ �˾� �޴��� �����Ѵ�.
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

	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	Long result = 0;
	//1.1.���泻���� ������
	if (this->changedStatus > 0){

		//1.1.1.�޼��� �ڽ��� ����
		result = MessageBox("���� ������ �����Ͻðڽ��ϱ�?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.�޼����ڽ����� �����ϱ⸦ ��������
		if (result == IDYES){

			//1.1.2.1.������ ���������϶�
			if (this->fileName != ""){
				//1.1.2.1.1.�������Ͽ� �����Ѵ�
				this->nsChartFormScrollBar->nsChartForm->Save(this->fileName);
			}
			//1.1.2.2.������ �������϶�
			else{

				//1.1.2.2.1.�ٸ��̸����� �����ϱ� â�� ����
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ 

					this->nsChartFormScrollBar->nsChartForm->Save(dlg.GetFileName());
				}
			}
		}
	}

	//1.2.�޼����ڽ��� ����� ��ҹ�ư�� �ƴѰ��
	if (result != IDCANCEL){
		//1.2.1.������
		
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
	
	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	int result = 0;
	//1.1.���泻���� ������
	if (*changedStatus2 > 0){

		//1.1.1.�޼��� �ڽ��� ����
		result = MessageBox("���� ������ �����Ͻðڽ��ϱ�?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.�޼����ڽ����� �����ϱ⸦ ��������
		if (result == IDYES){

			//1.1.2.1.������ ���������϶�
			if (*fileName2 != ""){
				//1.1.2.1.1.�������Ͽ� �����Ѵ�
				nsChartForm2->Save(*fileName2);
			}
			//1.1.2.2.������ �������϶�
			else{

				//1.1.2.2.1.�ٸ��̸����� �����ϱ� â�� ����
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
					// ���⼭ ���� ���� �۾�.
					// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

					nsChartForm2->Save(dlg.GetPathName());
				}
			}
		}
	}
	//1.2.�޼����ڽ� ����� ��Ұ� �ƴҰ��
	if (result != IDCANCEL){

		//1.2.1.����â�� ��� ������ �����
		nsChartForm2->EditMenu('A');
		nsChartForm2->EditMenu(VK_DELETE);
		*changedStatus2 = 0;
		*fileName2 = "";
		this->Invalidate();
	}

	nsChartForm2->SetFocus();
}

void FlyingDuckChart::OpenMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2){
	
	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}
	
	int result = 0;
	//1.1.���泻���� ������
	if (*changedStatus2 > 0){

		//1.1.1.�޼��� �ڽ��� ����
		result = MessageBox("���� ������ �����Ͻðڽ��ϱ�?", "FlyingDuckChart", MB_YESNOCANCEL);

		//1.1.2.�޼����ڽ����� �����ϱ⸦ ��������
		if (result == IDYES){

			//1.1.2.1.������ ���������϶�
			if (*fileName2 != ""){
				//1.1.2.1.1.�������Ͽ� �����Ѵ�
				nsChartForm2->Save(*fileName);
			}
			//1.1.2.2.������ �������϶�
			else{

				//1.1.2.2.1.�ٸ��̸����� �����ϱ� â�� ����
				CFileDialog dlg(false, "txt", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Text File(*.txt)|*.txt||");

				if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
					// ���⼭ ���� ���� �۾�.
					// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

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
			//1.1.2.3.����â�� ��� ������ �����
			nsChartForm2->EditMenu('A');
			nsChartForm2->EditMenu(VK_DELETE);
			//1.1.2.4. �ε��Ѵ�
			nsChartForm2->Load(dlg.GetPathName());
			*fileName2 = dlg.GetPathName();
			*changedStatus2 = 0;
			this->Invalidate();

		}
	}
	nsChartForm2->SetFocus();
}


void FlyingDuckChart::SaveMenu(Long *changedStatus2, CString *fileName2, NSChartForm *nsChartForm2){
	
	//editâ�� ���������� ������ ������.
	if (this->nsChartFormScrollBar->nsChartForm->form != NULL) {

		this->nsChartFormScrollBar->nsChartForm->form->OnClose();

	}

	if (*fileName2 == ""){

		CFileDialog dlg(false, "txt", NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Text File(*.txt)|*.txt||");

		if (dlg.DoModal() == IDOK){ // OK �� �ϴ� ���� ���� ��ΰ� ����ȴ�.
			// ���⼭ ���� ���� �۾�.
			// ���⼭ ��� dlg.GetFileName() �Լ��� ��� ��η��� ���� �̸��� ��ȯ�� �ش�.

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

