//Iteration.h
#ifndef _ITERATION_H
#define _ITERATION_H
#include "Block.h"

class Visitor;

class Iteration : public Block{

public:

	Iteration();
	Iteration(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY);
	Iteration(const Iteration& source);
	virtual ~Iteration();
	virtual Structure *Clone(); //Prototype pattern
	virtual string GetString();
	virtual void Accept(Visitor *visitor, CDC *pDC);
	virtual void AcceptPrint(Visitor *visitor, CDC *pDC);
	//프린트 미리보기 그리기
	virtual void Accept(Visitor *visitor, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC);
	virtual void Accept(Visitor *visitor, NSChart *temp, Long(*integers), Long *index, Long *count);
	virtual void Accept(Visitor *visitor, Structure* *indexStructure, Long x, Long y);
	virtual void Accept(Visitor *visitor, Long x, Long y);
	virtual void Accept(Visitor *visitor, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	virtual void Accept(Visitor *visitor);
	Iteration& operator=(const Iteration& source);
	virtual void Correct(Long x, Long y);
	virtual void Correct(Long x, Long y, Long width, Long height);
	virtual void CorrectXWidth(Long x, Long width);
	virtual void CorrectWidth(Long width);
	virtual void CorrectSize(Long x, Long y, Long checkingResult);
	virtual void CorrectContents(string object);
	virtual void Accept(Visitor *visitor, NSChart *nsChart, Structure *structure);
	virtual void Accept1(Visitor *visitor, Long x, Long midX, Long previousX);


	virtual void Accept(Visitor *visitor, Long maxWidth, NSChart *nsChart);
	virtual void AutoCorrect(Long x, Long y, Long width, Long height);


};

#endif //_ITERATION_H