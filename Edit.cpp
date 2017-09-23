//Edit.cpp
#include "Edit.h"
#include "Text.h"
#include "NSChartForm.h"
#include "NSChart.h"
#include "Structure.h"
#include "Case.h"
#include "Selection.h"
#include "Sequence.h"
#include "Iteration.h"
#include "Tokenizer.h"
#include "SelectedStructure.h"
#include "CheckReadySequence.h"
#include <imm.h>
#include <afxmsg_.h>
#include <string>
using namespace std;

#define IsSHIFTPressed() ( 0x8000 ==(GetKeyState(VK_SHIFT) & 0x8000 ))
#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))

#pragma warning(disable:4996)
#pragma comment(lib, "Imm32")

BEGIN_MESSAGE_MAP(Edit, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_MESSAGE(WM_IME_CHAR, OnHangeulChar)
	ON_MESSAGE(WM_IME_COMPOSITION, OnHangeulComposition)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnHangeulStartComposition)
END_MESSAGE_MAP()

Edit::Edit(){

}

Edit::~Edit() {

}


int Edit::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	this->x = 0;
	this->y = 0;
	this->current = 0;
	this->startX = -1;
	this->startY = -1;
	this->startIndex = 0;
	
	this->copyString = NULL;
	this->startCopy = false;
	this->isClicked = false;

	this->startMakeBlock = false;
	this->bComp = false;

	this->text = new Text;
	this->cListBox = NULL;
	this->count = 0;
	this->indexes = new string[100];
	this->sameWordIndexes = new Long[100];
	this->sameWordCount = 0;

	CreateSolidCaret(2, 15);
	//SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	//ShowCaret();

	//��Ʈ �����쿡�� Ŭ���� ������ ���ڿ��� �����´�
	Long j = 0;
	//Structure *structure;
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	hNSChartForm->editOpen = true;
	this->structure = hNSChartForm->selectedStructure->GetAt(0);
	string str;
	char *cha = NULL;
	this->checkingPoint = hNSChartForm->checkingResult;

	//���õ����� ���̽������̸�
	if (dynamic_cast<Case *>(this->structure)) {
		//���̽� ������ �ڽ��� Ŭ���ϸ�
		if (this->checkingPoint >= 12) {
			j = this->checkingPoint - 12;
			str = dynamic_cast<Case *>(this->structure)->GetCaseString(j);
			cha = new char[str.length() + 1];
			strcpy(cha, str.c_str());

			//this->text->Paste(this->current, cha);
			//this->x = this->text->GetColumn();
			//this->y = this->text->GetRow();
			//this->current = this->text->GetCurrent();
		}
		//���̽������� ���ڸ� ���� ��
		else {

			str = dynamic_cast<Case *>(this->structure)->GetContents().c_str();
			cha = new char[str.length() + 1];
			strcpy(cha, str.c_str());
			//this->text->Paste(this->current, cha);

		}
	}
	//���̽� ������ ������ �ٸ� ������ �������� ��
	else {

		str = structure->GetContents().c_str();
		cha = new char[str.length() + 1];
		strcpy(cha, str.c_str());
		//this->text->Paste(this->current, cha);
	}

	this->text->Paste(this->current, cha);
	this->x = this->text->GetColumn();
	this->y = this->text->GetRow();
	this->current = this->text->GetCurrent();
	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));

	if (cha != NULL) {
		delete[] cha;
	}


	this->cListBoxCurrent = -1;

	this->Invalidate();
	hNSChartForm->Invalidate();
	return 0;
}


void Edit::OnPaint(){
	//�׸� �׸��� �غ�
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);
	//��Ʈ�� ������
	CFont myfont;
	myfont.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
	CFont *pOldFont = pDC->SelectObject(&myfont);

	Long i = 0;
	Long j = 0;
	Long textX = 0;
	Long textY = 0;

	//���ڸ� �����ִ� �۾�
	char(*line) = 0;
	line = new char[256];
	line[0] = '\0';

	//text�� length��ŭ �ݺ��Ѵ�.
	while (i < this->text->GetLength()){
		//���� ĳ������ �������� �ƴϸ�
		if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
			//line�� ��� �־��ش�.
			line[j] = this->text->GetAt(i);
			line[j + 1] = '\0';
			j++;
		}

		//���� �ٲ��
		else if (this->text->GetAt(i) == '\r'){
			j = 0;
		}

		else if (this->text->GetAt(i) == '\n'){
			delete[] line;
			line = new char[256];
			line[0] = '\0';
			textY += 15;
		}

		//�� �پ� �����ش�.
		pDC->TextOut(textX, textY, CString(line));
		i++;
	}

	//������� ������ ĳ����
	char(*block) = NULL;
	block = new char[256];

	//���� �����ִ� �۾�
	if (this->startMakeBlock == true && this->startIndex != this->current){
		Long index = this->startIndex;
		Long currentIndex = this->current;
		Long temp = 0;
		i = 0;
		textX = 0;
		textY = 0;

		//������ �������� �����ش�.
		pDC->SetBkColor(RGB(0, 0, 250));
		pDC->SetTextColor(RGB(255, 255, 255));

		//�տ������� ����� ������� ��
		if (index < currentIndex){
			//���� ���� ��ǥ�� ���Ѵ�.
			while (i < index){
				if (this->text->GetAt(i) == '\r'){
					textY += 15;
				}
				else if (this->text->GetAt(i) == '\n'){
					textX = 0;
				}
				else if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
					textX += 6;
				}
				i++;
			}

			j = 0;
			//���� ����� �� ����
			while (index < currentIndex){

				if (this->text->GetAt(index) != '\r' && this->text->GetAt(index) != '\n'){
					block[j] = this->text->GetAt(index);
					block[j + 1] = '\0';
					j++;
					pDC->TextOut(textX, textY, CString(block));
				}

				else if (this->text->GetAt(index) == '\r'){
					delete[] block;
					block = new char[256];
					j = 0;
					textY += 15;
					textX = 0;
				}

				index++;
			}
		}
		//�ڿ������� ����� ������� ��
		else{
			//���� ���� ��ǥ�� ���Ѵ�.
			while (i < currentIndex){
				if (this->text->GetAt(i) == '\r'){
					textY += 15;
				}
				else if (this->text->GetAt(i) == '\n'){
					textX = 0;
				}
				else if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
					textX += 6;
				}
				i++;
			}

			j = 0;
			//���� ����� �� ����
			while (currentIndex < index){

				if (this->text->GetAt(currentIndex) != '\r' && this->text->GetAt(currentIndex) != '\n'){
					block[j] = this->text->GetAt(currentIndex);
					block[j + 1] = '\0';
					j++;
					pDC->TextOut(textX, textY, CString(block));
				}

				else if (this->text->GetAt(currentIndex) == '\r'){
					delete[] block;
					block = new char[256];
					j = 0;
					textY += 15;
					textX = 0;
				}

				currentIndex++;
			}
		}

	}

	ShowCaret();

	ReleaseDC(pDC);

	EndPaint(&Ps);

	if (line != NULL){
		delete[] line;
	}

	if (block != NULL){
		delete[] block;
	}
}

void Edit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();

	//up��ư�̳� down ��ư�� �ƴҶ� 7.27
	if (nChar != VK_UP && nChar != VK_DOWN  && nChar != VK_RETURN){

		//����Ʈ�ڽ��� �����ִ� �����϶�
		if (this->cListBox != NULL){
			//����Ʈ�ڽ��� �ݴ´�.
			delete this->cListBox;
			this->cListBox = NULL;
		}
	}

	switch (nChar) {
	case VK_UP:
		if (IsSHIFTPressed() == TRUE) {
			////////////////////////////////7.27
			//����Ʈ�ڽ��� �����ִ� �����϶�
			if (this->cListBox != NULL){
				//����Ʈ�ڽ��� �ݴ´�.
				delete this->cListBox;
				this->cListBox = NULL;
			}

			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			if (this->y > 0) {
				this->y -= 1;

				Long i = 0;
				Long j = 0;
				Long index;
				Long byteCount[256];
				//1. -1�� ���� ������ �������� �ݺ��Ѵ�.
				while (i < this->x){
					//1.1. �� ���� �迭���� ���°���� �˾Ƴ���.
					index = this->text->Position(this->y, i);
					//1.2. index������ �ִ� ���ڰ� �ѱ��̸�
					if (this->IsDBCS(index) == TRUE){
						//1.2.1. �����迭�� 2�� �ִ´�.
						byteCount[j] = 2;
						j++;
						i++;
					}
					//1.3. index��°�� �ִ� ���ڰ� �ѱ��� �ƴ� �ٸ� �����̸�
					else{
						byteCount[j] = 1;
						j++;
					}
					i++;
				}
				//2. �����迭 �������� 0�� �־��ش�.
				byteCount[j] = 0;

				i = 0;
				//3. ��ġ�� ���� ���� �ʱ�ȭ�����ش�.
				this->x = 0;
				//4. �����迭�� 0�� �ƴҶ����� �ݺ��Ѵ�.
				while (byteCount[i] != 0){
					//4.1. �ʱ�ȭ�� ���� �����迭�� ����� ���� ���ʷ� �־��༭ ������Ų��.
					this->x = this->x + byteCount[i];
					i++;
				}

			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);
			}
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			/////////////////////////7.27
			//����Ʈ �ڽ��� ����������
			if (this->cListBox != NULL){
				//����Ʈ�ڽ� Ŀ���� ������
				if (this->cListBoxCurrent == -1){

					//���� �Ʒ��� Ŀ���� �д�
					this->cListBox->SetCurSel(this->sameWordCount - 1);
					this->cListBoxCurrent = this->sameWordCount - 1;
				}
				//Ŀ����ġ�� ���� ��������������
				else if (this->cListBoxCurrent > 0){
					//�ϳ��� Ŀ���� �ø���
					this->cListBoxCurrent--;
					this->cListBox->SetCurSel(this->cListBoxCurrent);
				}
				////////////////////////////////7.27.

			}
			else{
				this->startMakeBlock = false;
				this->startCopy = false;
				//Caret �̵�
				if (this->y > 0) {
					this->y -= 1;

					Long i = 0;
					Long j = 0;
					Long index;
					Long byteCount[256];
					while (i < this->x){
						index = this->text->Position(this->y, i);
						if (this->IsDBCS(index) == TRUE){
							byteCount[j] = 2;
							j++;
							i++;
						}
						else{
							byteCount[j] = 1;
							j++;
						}
						i++;
					}
					byteCount[j] = 0;

					i = 0;
					this->x = 0;
					while (byteCount[i] != 0){
						this->x = this->x + byteCount[i];
						i++;
					}

				}
				if (this->x > this->text->GetRowLength(this->y)) {
					this->x = this->text->GetRowLength(this->y);
				}
				//String �̵�
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);

				this->current = this->text->Position(this->y, this->x);
			}
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;

	case VK_DOWN:
		if (IsSHIFTPressed() == TRUE) {
			///////////////////7.27
			//����Ʈ�ڽ��� �����ִ� �����϶�
			if (this->cListBox != NULL){
				//����Ʈ�ڽ��� �ݴ´�.
				delete this->cListBox;
				this->cListBox = NULL;
				//////////////////7.27
			}

			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			//Caret �̵�
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;//String �̵�

				Long i = 0;
				Long j = 0;
				Long index;
				Long byteCount[256];
				while (i < this->x){
					index = this->text->Position(this->y, i);
					if (this->IsDBCS(index) == TRUE){
						byteCount[j] = 2;
						j++;
						i++;
					}
					else{
						byteCount[j] = 1;
						j++;
					}
					i++;
				}
				byteCount[j] = 0;

				i = 0;
				this->x = 0;
				while (byteCount[i] != 0){
					this->x = this->x + byteCount[i];
					i++;
				}
				
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);//String �̵�
				
			}
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			//////////////////////7.27
			//����Ʈ�ڽ��� ���� �ִ� �����϶�
			if (this->cListBox != NULL){
				//����Ʈ�ڽ� Ŀ���� ǥ�õǾ�����������
				if (this->cListBoxCurrent == -1){

					//���� ���� Ŀ���� �д�
					this->cListBox->SetCurSel(0);
					this->cListBoxCurrent = 0;
				}
				//Ŀ���� ���� �ؿ� ����������
				else if (this->cListBoxCurrent < this->sameWordCount - 1){
					//Ŀ���� �ϳ��� ������
					this->cListBoxCurrent++;
					this->cListBox->SetCurSel(this->cListBoxCurrent);
					////////////////////7.27
				}
			}
			else{
				this->startMakeBlock = false;
				this->startCopy = false;
				//Caret �̵�
				if (this->y < this->text->GetMaxRow()) {
					this->y += 1;

					Long i = 0;
					Long j = 0;
					Long index;
					Long byteCount[256];
					while (i < this->x){
						index = this->text->Position(this->y, i);
						if (this->IsDBCS(index) == TRUE){
							byteCount[j] = 2;
							j++;
							i++;
						}
						else{
							byteCount[j] = 1;
							j++;
						}
						i++;
					}
					byteCount[j] = 0;

					i = 0;
					this->x = 0;
					while (byteCount[i] != 0){
						this->x = this->x + byteCount[i];
						i++;
					}

				}
				if (this->x > this->text->GetRowLength(this->y)) {
					this->x = this->text->GetRowLength(this->y);
				}
				//String �̵�
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);

				this->current = this->text->Position(this->y, this->x);
			}
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;

	case VK_LEFT:

		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = true;
				this->startCopy = true;
			}

			if (this->x > 0) {
				if (this->IsDBCS(this->current - 1) == TRUE){
					this->x -= 2;
				}
				else{
					this->x -= 1;
				}
			}
			else if (this->x == 0 && this->y > 0) {
				this->y -= 1;
				this->x = this->text->GetRowLength(this->y);
			}
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->x > 0) {
				if (this->IsDBCS(this->current - 1) == TRUE){
					this->x -= 2;
				}
				else{
					this->x -= 1;
				}
			}
			else if (this->x == 0 && this->y > 0) {
				this->y -= 1;
				this->x = this->text->GetRowLength(this->y);
			}
			//String �̵�
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;

	case VK_RIGHT:

		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			if (this->x < this->text->GetRowLength(this->y)) {
				if (this->IsDBCS(this->current) == TRUE){
					this->x += 2;
				}
				else{
					this->x += 1;
				}
			}
			else if (this->x == this->text->GetRowLength(this->y) && this->y < this->text->GetMaxRow()) {
				this->y += 1;
				this->x = 0;
			}
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->x < this->text->GetRowLength(this->y)) {
				if (this->IsDBCS(this->current) == TRUE){
					this->x += 2;
				}
				else{
					this->x += 1;
				}
			}
			else if (this->x == this->text->GetRowLength(this->y) && this->y < this->text->GetMaxRow()) {
				this->y += 1;
				this->x = 0;
			}
			//String �̵�
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;

	case 'A':

		if (IsCTRLPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startMakeBlock = true;

				this->startY = 0;
				this->startX = 0;
				this->startIndex = 0;

				this->y = this->text->GetMaxRow();
				this->x = this->text->GetRowLength(this->y);

				this->startMakeBlock = true;
				this->startCopy = true;

				this->current = this->text->Position(this->y, this->x);

				SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
				this->Invalidate();
				hNSChartForm->Invalidate();

			}
		}


		break;
	case 'C':

		if (IsCTRLPressed() == TRUE) {
			if (this->startCopy == true) {
				if (this->copyString != NULL) {
					delete[] this->copyString;
				}
				this->copyString = this->text->Copy(this->startY, this->startX);
				//////NSChartForm�� ���� �����س��´�.////////////////
				Long i = 0;
				while (this->copyString[i] != '\0'){
					hNSChartForm->editCopy[i] = this->copyString[i];
					i++;
				}
				hNSChartForm->editCopy[i] = '\0';
				/////////////////////////////////////////////////////
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				this->current = this->text->Position(this->y, this->x);
			}
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();

		break;
	case 'X':

		if (IsCTRLPressed() == TRUE) {
			if (this->startCopy == true) {
				if (this->copyString != NULL) {
					delete[] this->copyString;
				}
				this->copyString = this->text->Cut(this->startY, this->startX);
				//////NSChartForm�� ���� �����س��´�.////////////////
				Long i = 0;
				while (this->copyString[i] != '\0'){
					hNSChartForm->editCopy[i] = this->copyString[i];
					i++;
				}
				hNSChartForm->editCopy[i] = '\0';
				/////////////////////////////////////////////////////
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				this->current = this->text->Position(this->y, this->x);

				this->startY = this->y;
				this->startX = this->x;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = false;
			}
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();

		break;
	case 'V':

		if (IsCTRLPressed() == TRUE) {
			if (this->copyString != NULL) {
				if (this->startMakeBlock == true) {
					this->x = this->startX;
					this->y = this->startY;

					this->current = this->text->Position(this->y, this->x);
					this->startMakeBlock = false;
					this->startCopy = false;

				}
				else {
					this->current = this->text->Paste(this->current, this->copyString);
					this->y = this->text->GetRow();
					this->x = this->text->GetColumn();

					//this->startY = this->y;
					//this->startX = this->x;
					this->startIndex = this->text->Position(this->y, this->x);
				}
			}
			/////////////////////NSChartForm�� ���� �ٿ����´�.////////////////////////////
			if (hNSChartForm->editCopy != NULL){
				if (this->startMakeBlock == true) {
					this->x = this->startX;
					this->y = this->startY;

					this->current = this->text->Position(this->y, this->x);
					this->startMakeBlock = false;
					this->startCopy = false;

				}
				else {
					this->current = this->text->Paste(this->current, hNSChartForm->editCopy);
					this->y = this->text->GetRow();
					this->x = this->text->GetColumn();

					//this->startY = this->y;
					//this->startX = this->x;
					this->startIndex = this->text->Position(this->y, this->x);
				}
			}
			///////////////////////////////////////////////////////////////////////////////

		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();

		break;
	case VK_HOME:

		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->startX);
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			/*else {
			this->startMakeBlock = false;
			this->startCopy = false;
			}*/
		}
		this->x = 0;
		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();

		break;
	case VK_END:

		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->startX);
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			/*else {
			this->startMakeBlock = false;
			this->startCopy = false;
			}*/
		}
		this->x = this->text->GetRowLength(this->y);
		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();

		break;
	case VK_BACK:

		if (this->isClicked == false && this->startMakeBlock == false) {
			if (this->y > 0 || this->x > 0){

				//���� �ϼ��� �ѱ��� ���ﶧ
				if (this->IsDBCS(this->current - 1) == TRUE){
					this->text->BackSpace(this->current);
					this->text->BackSpace(this->current - 1);
				}
				//�ѱ� �ƴ� ���ڸ� ���� ��
				else{
					this->current = this->text->BackSpace(this->current);;
				}

			}
			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();

		}
		else if (this->isClicked == false && this->startMakeBlock == true) {
			this->current = this->text->Erase(this->startY, this->startX);
			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();
			this->startMakeBlock = FALSE;
		}

		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;
	case VK_DELETE:
		if (this->isClicked == false && this->startMakeBlock == false) {
			//�ѱ��̸�
			if (this->IsDBCS(this->current) == TRUE){
				this->text->Delete(this->current);
				this->text->Delete(this->current);
			}
			//�ٸ� �����̿�
			else{
				this->text->Delete(this->current);
			}

		}
		else if (this->isClicked == false && this->startMakeBlock == true) {
			this->text->Erase(this->startY, this->startX);

			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();

			this->startMakeBlock = false;
		}

		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;

	default:
		break;
	}

}


void Edit::OnKeyUP(UINT nChar, UINT nRepCnet, UINT nFlags) {

}


void Edit::OnChar(UINT nChar, UINT nRepcnt, UINT nFlags){
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();

	CheckReadySequence *checkReadySequence = new CheckReadySequence;
	switch (nChar) {
		///////////////////////////////////////7.27
		//���͸� ġ�� ������ �� �����.
	case VK_RETURN:
		//����Ʈ�ڽ��� �����ִ� �����϶�
		if (this->cListBox != NULL){
			CString cstr;
			//����Ʈ�ڽ��� Ŀ���� ǥ�õǾ�������
			if (this->cListBoxCurrent != -1){
				//Ŀ���� ǥ�õǾ��ִ� ������ �д´�
				this->cListBox->GetText(this->cListBoxCurrent, cstr);
				//���ڸ� ����Ʈ�� ���δ�
				this->current = this->text->BackSpace(this->current);
				this->current = this->text->Paste(this->current, LPSTR(LPCTSTR(cstr)));
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				//����Ʈ�ڽ��� ���ش�
				delete this->cListBox;
				this->cListBox = NULL;
				////////////////////////////7.27
			}

		}
		else{


			if (this->startMakeBlock == true) {
				this->text->Erase(this->startY, this->startX);

				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();

				this->startMakeBlock = false;
			}

			this->text->Write(this->text->GetCurrent(), '\r');
			this->text->Write(this->text->GetCurrent(), '\n');
			this->x = 0;
			(this->y)++;
			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		//ShowCaret();
		this->Invalidate();
		hNSChartForm->Invalidate();
		break;
		//KeyDown���� ���ִ� back�� OnChar���� �ѹ� �޾������ �̻��� ���ڰ� �ȹ���.
	case VK_BACK:
		break;
		//ESC�� �θ��� Ŭ�ν��� �θ��鼭 CFrameWnd�� ���ڰ� ������.
	case VK_ESCAPE:
		this->OnClose();
		break;

		//�۾��� �۾����ִ� ��
	default:
		if (TRUE != IsCTRLPressed()) {
			if (this->startMakeBlock == true) {
				this->startMakeBlock = false;
			}
			if (this->startCopy == true) {
				this->startCopy = false;
			}
			this->text->Write(this->text->GetCurrent(), nChar);
			(this->x)++;
			this->current = this->text->Position(this->y, this->x);

			SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
			//ShowCaret();
			this->Invalidate();
			hNSChartForm->Invalidate();

			//����Ʈ�ڽ� ���鶧 7.27
			Long z = 0;
			Structure *readyStructure = NULL;
			bool autoComplete = false;


			//�غ��ȣ�� ã�´�
			checkReadySequence->GetReadySequence(hNSChartForm->nsChart, &readyStructure);



			//�غ��ȣ�� �����ϰ� ���������� �غ��ȣ�� ����������
			if (readyStructure != NULL && this->structure != readyStructure){
				autoComplete = true;


			}
			//�Ȱ��� ���ڸ� ���� ���ڿ����� �����´�
			char n = nChar;

			//����Ʈ�ڽ� �����ؼ� ���ڹڱ�
			if (nChar != VK_ESCAPE &&  autoComplete == true){
				checkReadySequence->ReadySequenceStrings(readyStructure, n, this->indexes, &this->count, this->sameWordIndexes, &this->sameWordCount);

				this->cListBox = new CListBox;
				this->cListBox->Create(WS_CHILD | WS_VISIBLE | LBS_DISABLENOSCROLL, CRect(this->x * 6, this->y * 15, this->x * 6 + 150, this->y * 15 + 100), this, 121);


				while (z < this->sameWordCount){
					this->cListBox->AddString((CString)this->indexes[this->sameWordIndexes[z]].c_str());
					z++;
				}
				this->cListBoxCurrent = -1;

			}
			/////////////////////////
		}

		break;
	}

	if (checkReadySequence != NULL){

		delete checkReadySequence;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
void Edit::OnLButtonDown(UINT nFlags, CPoint point){
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	//��ġ�� ����
	this->x = point.x / 6;
	this->y = point.y / 15;

	if (this->y > this->text->GetMaxRow()){
		this->y = this->text->GetMaxRow();
	}

	if (this->x > this->text->GetRowLength(this->y)){
		this->x = this->text->GetRowLength(this->y);
	}

	this->startX = this->x;
	this->startY = this->y;

	this->startIndex = this->text->Position(this->startY, this->startX);
	this->current = this->text->Position(this->y, this->x);

	this->startMakeBlock = true;
	this->isClicked = true;

	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	this->Invalidate();
	hNSChartForm->Invalidate();
}


void Edit::OnMouseMove(UINT nFlags, CPoint point){
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	if (this->startMakeBlock == true && this->isClicked == true){
		this->x = point.x / 6;
		this->y = point.y / 15;

		if (this->y > this->text->GetMaxRow()) {
			this->y = this->text->GetMaxRow();
		}
		if (this->x > this->text->GetRowLength(this->y)) {
			this->x = this->text->GetRowLength(this->y);
		}
		this->current = this->text->Position(this->y, this->x);
		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		hNSChartForm->Invalidate();
	}
}


void Edit::OnLButtonUp(UINT nFlags, CPoint point) {
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	this->x = point.x / 6;
	this->y = point.y / 15;
	/*
	Long maxRow = this->text->GetMaxRow();
	Long rowLength = this->text->GetRowLength(maxRow);

	if (this->y > maxRow && this->x > rowLength) {
	this->y = maxRow;
	this->x = rowLength;
	}
	else if (this->y > maxRow && this->x <= rowLength) {
	this->y = maxRow;
	}
	else if (this->y <= maxRow && this->x > this->text->GetRowLength(this->y)) {
	this->x = this->text->GetRowLength(this->y);
	}
	*/
	if (this->y > this->text->GetMaxRow()) {
		this->y = this->text->GetMaxRow();
	}
	if (this->x > this->text->GetRowLength(this->y)) {
		this->x = this->text->GetRowLength(this->y);
	}

	if (this->x == this->startX && this->y == this->startY) {
		this->startCopy = false;
		this->startMakeBlock = false;
		this->startIndex = this->text->Position(this->y, this->x);
	}
	else if (this->startMakeBlock == true && this->isClicked == true) {
		this->startCopy = true;
	}
	this->current = this->text->Position(this->y, this->x);
	this->isClicked = false;

	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	this->Invalidate();
	hNSChartForm->Invalidate();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//�ѱ� �Է�
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Edit::IsDBCS(Long index){
	return (IsDBCSLeadByte(this->text->GetAt(index)));
}

LRESULT Edit::OnHangeulChar(WPARAM wParam, LPARAM lParam){
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();

	TCHAR szChar[3];

	if (::IsDBCSLeadByte((BYTE)(wParam >> 8))) {
		szChar[0] = HIBYTE(LOWORD(wParam));
		szChar[1] = LOBYTE(LOWORD(wParam));
		szChar[2] = '\0';
	}
	else {
		szChar[0] = (BYTE)wParam;
		szChar[1] = 0;
	}
	if (this->bComp == true) {
		this->current -= 2;
		this->text->HangeulDelete(this->current);
	}
	this->text->HangeulPaste(this->current, szChar);
	this->x += 2;
	this->current += lstrlen(szChar);
	this->bComp = false;

	this->InvalidateRect(NULL, TRUE);
	hNSChartForm->Invalidate();
	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	return 0;
}

LRESULT Edit::OnHangeulComposition(WPARAM wParam, LPARAM lParam){
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	HIMC hImc;
	TCHAR *buffer;
	Long count;

	hImc = ::ImmGetContext(this->m_hWnd);
	if (lParam & GCS_COMPSTR) {
		count = ::ImmGetCompositionString(hImc, GCS_COMPSTR, NULL, 0);
		buffer = new TCHAR[count + 1];
		ImmGetCompositionString(hImc, GCS_COMPSTR, buffer, count);
		buffer[count] = '\0';
		if (this->bComp == true) {
			this->current -= 2;
			this->text->HangeulDelete(this->current);
		}
		if (count == 0) {
			this->bComp = false;
		}
		else {
			this->bComp = true;
		}

		this->text->HangeulWrite(this->current, buffer);
		this->current += lstrlen(buffer);

		::ImmReleaseContext(this->m_hWnd, hImc);

		if (buffer != NULL){
			delete[] buffer;
		}
		this->InvalidateRect(NULL, TRUE);
		hNSChartForm->Invalidate();
		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	}

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT Edit::OnHangeulStartComposition(WPARAM wParam, LPARAM lParam){
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//�ѱ� �Է�
////////////////////////////////////////////////////////////////////////////////////////////////


void Edit::OnClose() {

	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	hNSChartForm->editOpen = false;

	string str;
	char(*cha) = NULL;
	cha = this->text->GetString();
	str = cha;
	if (cha != NULL) {
		delete cha;
	}

	if (dynamic_cast<Case *>(this->structure)) {
		if (this->checkingPoint >= 12) {
			Long j = this->checkingPoint - 12;
			dynamic_cast<Case *>(this->structure)->CaseStringModify(j, str);
		}
		else {

			dynamic_cast<Case *>(this->structure)->CorrectContents(str);
		}


	}
	else {
		this->structure->CorrectContents(str);
	}

	hNSChartForm->Invalidate();

	if (this->text != NULL) {

		delete this->text;
	}

	if (this->copyString != NULL) {
		delete[] this->copyString;
	}

	//////////////////
	///////////////// ����Ʈ�ڽ� ���ú����� �Ҵ����� 7.27
	if (this->cListBox != NULL){

		delete this->cListBox;
	}
	if (this->sameWordIndexes != NULL){

		delete[] this->sameWordIndexes;
	}
	if (this->indexes != NULL){

		delete[] this->indexes;
	}
	///
	CWnd::DestroyWindow();


	if (hNSChartForm->form != NULL) {
		delete hNSChartForm->form;
		hNSChartForm->form = NULL;
	}

}

