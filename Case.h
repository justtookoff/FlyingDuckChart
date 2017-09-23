//Case.h
#ifndef _CASE_H
#define _CASE_H
#include "Block.h"
#include <string>

class Visitor;
class String;
class Case : public Block{

public:
	Case();
	Case(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY, Long count);
	Case(const Case& source);
	virtual ~Case();
	virtual Structure* Clone();
	virtual string GetString();
	virtual void Accept(Visitor *visitor, CDC *pDC);
	virtual void AcceptPrint(Visitor *visitor, CDC *pdC);
	//프린트 미리보기 그리기
	virtual void Accept(Visitor *visitor, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	virtual void Accept(Visitor *visitor, NSChart *temp, Long(*integers), Long *index, Long *count);
	virtual void Accept(Visitor *visitor, Structure* *indexStructure, Long x, Long y);
	virtual void Accept(Visitor *visitor, Long x, Long y);
	virtual void Accept(Visitor *visitor, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	virtual void Accept(Visitor *visitor);
	Case& operator=(const Case& source);
	virtual void Correct(Long x, Long y);
	virtual void Correct(Long x, Long y, Long width, Long height);
	virtual void CorrectXWidthMidX(Long x, Long width, Long midX);
	virtual void CorrectWidth(Long width);
	virtual void CorrectSize(Long x, Long y, Long checkingResult);
	virtual void CorrectCaseCount(Long count);

	Long CaseStringModify(Long index, string object);
	virtual void CorrectContents(string object);
	virtual void Accept(Visitor *visitor, NSChart *nsChart, Structure *structure);
	virtual void Accept1(Visitor *visitor, Long x, Long midX, Long previousX);

	virtual void Accept(Visitor *visitor, Long maxWidth, NSChart *nsChart);
	virtual void AutoCorrect(Long x, Long y, Long width, Long height);
	
	string GetCaseString(Long index);
	Long GetCount() const;
	//Long GetLength() const;
	//Long GetCapacity() const;

private:
	Array<string> caseStrings;
	Long count;
	//Long length;
	//Long capacity;
};

inline string Case::GetCaseString(Long index) {
	return this->caseStrings.GetAt(index);
}

inline Long Case::GetCount() const{
	return this->count;
}

//inline Long Case::GetLength() const{
//	return this->length;
//}
//
//inline Long Case::GetCapacity() const{
//	return this->capacity;
//}

#endif //_CASE_H