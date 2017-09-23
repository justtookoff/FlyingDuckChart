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

	//차트 윈도우에서 클릭한 도형의 문자열을 가져온다
	Long j = 0;
	//Structure *structure;
	NSChartForm *hNSChartForm = (NSChartForm*)CWnd::GetParent();
	hNSChartForm->editOpen = true;
	this->structure = hNSChartForm->selectedStructure->GetAt(0);
	string str;
	char *cha = NULL;
	this->checkingPoint = hNSChartForm->checkingResult;

	//선택도형이 케이스구조이면
	if (dynamic_cast<Case *>(this->structure)) {
		//케이스 구조의 자식을 클릭하면
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
		//케이스구조에 문자를 넣을 때
		else {

			str = dynamic_cast<Case *>(this->structure)->GetContents().c_str();
			cha = new char[str.length() + 1];
			strcpy(cha, str.c_str());
			//this->text->Paste(this->current, cha);

		}
	}
	//케이스 구조를 제외한 다른 구조를 선택했을 때
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
	//그림 그리기 준비
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);
	//폰트를 설정함
	CFont myfont;
	myfont.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont = pDC->SelectObject(&myfont);

	Long i = 0;
	Long j = 0;
	Long textX = 0;
	Long textY = 0;

	//글자를 보여주는 작업
	char(*line) = 0;
	line = new char[256];
	line[0] = '\0';

	//text의 length만큼 반복한다.
	while (i < this->text->GetLength()){
		//리턴 캐리지나 뉴라인이 아니면
		if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
			//line에 계속 넣어준다.
			line[j] = this->text->GetAt(i);
			line[j + 1] = '\0';
			j++;
		}

		//줄이 바뀌면
		else if (this->text->GetAt(i) == '\r'){
			j = 0;
		}

		else if (this->text->GetAt(i) == '\n'){
			delete[] line;
			line = new char[256];
			line[0] = '\0';
			textY += 15;
		}

		//한 줄씩 보여준다.
		pDC->TextOut(textX, textY, CString(line));
		i++;
	}

	//블락으로 엮어줄 캐릭터
	char(*block) = NULL;
	block = new char[256];

	//블럭을 보여주는 작업
	if (this->startMakeBlock == true && this->startIndex != this->current){
		Long index = this->startIndex;
		Long currentIndex = this->current;
		Long temp = 0;
		i = 0;
		textX = 0;
		textY = 0;

		//색깔을 반전으로 입혀준다.
		pDC->SetBkColor(RGB(0, 0, 250));
		pDC->SetTextColor(RGB(255, 255, 255));

		//앞에서부터 블락을 만들었을 때
		if (index < currentIndex){
			//블럭의 시작 좌표를 구한다.
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
			//블럭이 생기는 곳 까지
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
		//뒤에서부터 블락을 만들었을 때
		else{
			//블럭의 시작 좌표를 구한다.
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
			//블럭이 생기는 곳 까지
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

	//up버튼이나 down 버튼이 아닐때 7.27
	if (nChar != VK_UP && nChar != VK_DOWN  && nChar != VK_RETURN){

		//리스트박스가 켜져있는 상태일때
		if (this->cListBox != NULL){
			//리스트박스를 닫는다.
			delete this->cListBox;
			this->cListBox = NULL;
		}
	}

	switch (nChar) {
	case VK_UP:
		if (IsSHIFTPressed() == TRUE) {
			////////////////////////////////7.27
			//리스트박스가 켜져있는 상태일때
			if (this->cListBox != NULL){
				//리스트박스를 닫는다.
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
				//1. -1된 행의 열보다 작은동안 반복한다.
				while (i < this->x){
					//1.1. 그 열의 배열에서 몇번째인지 알아낸다.
					index = this->text->Position(this->y, i);
					//1.2. index번쨰에 있는 글자가 한글이면
					if (this->IsDBCS(index) == TRUE){
						//1.2.1. 정수배열에 2를 넣는다.
						byteCount[j] = 2;
						j++;
						i++;
					}
					//1.3. index번째에 있는 글자가 한글이 아닌 다른 글자이면
					else{
						byteCount[j] = 1;
						j++;
					}
					i++;
				}
				//2. 정수배열 마지막에 0을 넣어준다.
				byteCount[j] = 0;

				i = 0;
				//3. 위치된 행의 열을 초기화시켜준다.
				this->x = 0;
				//4. 정수배열이 0이 아닐때까지 반복한다.
				while (byteCount[i] != 0){
					//4.1. 초기화된 열에 정수배열에 저장된 것을 차례로 넣어줘서 증가시킨다.
					this->x = this->x + byteCount[i];
					i++;
				}

			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);
			}
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			/////////////////////////7.27
			//리스트 박스가 켜져있을때
			if (this->cListBox != NULL){
				//리스트박스 커서가 없을때
				if (this->cListBoxCurrent == -1){

					//제일 아래에 커서를 둔다
					this->cListBox->SetCurSel(this->sameWordCount - 1);
					this->cListBoxCurrent = this->sameWordCount - 1;
				}
				//커서위치가 제일 위에있지않을때
				else if (this->cListBoxCurrent > 0){
					//하나씩 커서를 올린다
					this->cListBoxCurrent--;
					this->cListBox->SetCurSel(this->cListBoxCurrent);
				}
				////////////////////////////////7.27.

			}
			else{
				this->startMakeBlock = false;
				this->startCopy = false;
				//Caret 이동
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
				//String 이동
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
			//리스트박스가 켜져있는 상태일때
			if (this->cListBox != NULL){
				//리스트박스를 닫는다.
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
			//Caret 이동
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;//String 이동

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
				this->x = this->text->GetRowLength(this->y);//String 이동
				
			}
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			//////////////////////7.27
			//리스트박스가 켜져 있는 상태일때
			if (this->cListBox != NULL){
				//리스트박스 커서가 표시되어있지않을때
				if (this->cListBoxCurrent == -1){

					//제일 위에 커서를 둔다
					this->cListBox->SetCurSel(0);
					this->cListBoxCurrent = 0;
				}
				//커서가 제일 밑에 있지않을때
				else if (this->cListBoxCurrent < this->sameWordCount - 1){
					//커서를 하나씩 내린다
					this->cListBoxCurrent++;
					this->cListBox->SetCurSel(this->cListBoxCurrent);
					////////////////////7.27
				}
			}
			else{
				this->startMakeBlock = false;
				this->startCopy = false;
				//Caret 이동
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
				//String 이동
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
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
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
			//String 이동
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
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
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
			//String 이동
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
				//////NSChartForm에 글을 저장해놓는다.////////////////
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
				//////NSChartForm에 글을 저장해놓는다.////////////////
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
			/////////////////////NSChartForm에 글을 붙여놓는다.////////////////////////////
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

				//조립 완성된 한글을 지울때
				if (this->IsDBCS(this->current - 1) == TRUE){
					this->text->BackSpace(this->current);
					this->text->BackSpace(this->current - 1);
				}
				//한글 아닌 글자를 지울 때
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
			//한글이면
			if (this->IsDBCS(this->current) == TRUE){
				this->text->Delete(this->current);
				this->text->Delete(this->current);
			}
			//다른 문자이연
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
		//엔터를 치면 한줄이 더 생긴다.
	case VK_RETURN:
		//리스트박스가 켜져있는 상태일때
		if (this->cListBox != NULL){
			CString cstr;
			//리스트박스에 커서가 표시되어있을때
			if (this->cListBoxCurrent != -1){
				//커서가 표시되어있는 문자을 읽는다
				this->cListBox->GetText(this->cListBoxCurrent, cstr);
				//문자를 에디트에 붙인다
				this->current = this->text->BackSpace(this->current);
				this->current = this->text->Paste(this->current, LPSTR(LPCTSTR(cstr)));
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				//리스트박스를 없앤다
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
		//KeyDown에서 해주는 back을 OnChar에서 한번 받아줘야지 이상한 문자가 안박힘.
	case VK_BACK:
		break;
		//ESC를 부르면 클로스를 부르면서 CFrameWnd에 글자가 박힌다.
	case VK_ESCAPE:
		this->OnClose();
		break;

		//글쓰는 작업해주는 곳
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

			//리스트박스 만들때 7.27
			Long z = 0;
			Structure *readyStructure = NULL;
			bool autoComplete = false;


			//준비기호를 찾는다
			checkReadySequence->GetReadySequence(hNSChartForm->nsChart, &readyStructure);



			//준비기호가 존재하고 켜진도형과 준비기호가 같지않을때
			if (readyStructure != NULL && this->structure != readyStructure){
				autoComplete = true;


			}
			//똑같은 문자를 가진 문자열들을 가져온다
			char n = nChar;

			//리스트박스 생성해서 글자박기
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
	//위치를 지정
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
//한글 입력
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
//한글 입력
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
	///////////////// 리스트박스 관련변수들 할당해제 7.27
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

