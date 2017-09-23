//Block.h
#ifndef _BLOCK_H
#define _BLOCK_H
#include "Array.h"
#include "Structure.h"

class NSChart;
class CopyStructure;
class Block :public Structure {

public:
	Block();
	Block(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY);
	Block(const Block& source);
	virtual ~Block();
	bool IsEqual(const Block& other);
	bool IsNotEqual(const Block& other);
	bool operator == (const Block& other);
	bool operator !=(const Block& other);
	virtual Structure* GetChild(Long index);
	//virtual Long Correct(Long index, string contents);
	virtual void Correct(Long x, Long y){}
	virtual void Correct(Long x, Long y, Long width, Long height){}
	virtual void CorrectXWidth(Long x, Long width){}
	virtual void CorrectXWidthMidX(Long x, Long width, Long midX){}
	virtual void CorrectWidth(Long width){}
	virtual void CorrectSize(Long x, Long y, Long checkingResult){}
	virtual void CorrectCaseCount(Long count){}
	virtual Long Add(Structure *structure);
	virtual Structure* Find(Long x, Long y);
	//�巡���ؼ� ���߼����ϱ�
	virtual void DragFind(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY);
	//NSChart ��ü�� x�� y��ǥ�� �����Ѵ�.
	virtual void Arrange();
	//block�� �ڽĵ��� �����ϴ� �Լ�
	virtual void ArrangeChild();
	virtual Structure* Clone();
	//����Ʈ �̸����� �׸���
	virtual void Accept(Visitor *visitor, Long topLeftX, Long topLeftY, Long bottomRightX, Long bottomRightY, CDC *pDC){}
	//����Ʈ�� ���� �׸���
	virtual void AcceptPrint(Visitor *visitor, CDC *pDC) {}
	//�׸���
	virtual void Accept(Visitor *visitor, CDC *pDC) {}
	virtual void Accept(Visitor *visitor, NSChart *temp, Long(*integers), Long *index, Long *count) {}
	virtual void Accept(Visitor *visitor, Structure* *indexStructure, Long x, Long y){}
	virtual void Accept(Visitor *visitor, Long x, Long y){}
	//�巡���ؼ� ���߼����ϱ�
	virtual void Accept(Visitor *visitor, GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){}
	//Delete�Ҷ� nsChart�� �ִ°͵� �ϴ� ������ ��!!
	virtual void Accept(Visitor *visitor){}
	virtual string GetString() { return 0; }
	//Delete�� ���� Method
	virtual Long Search(Structure *structure);
	virtual Long Delete(Long index);
	/////////////////////////////////////////
	//���� ������ ���� Method
	virtual Long Insert(Long index, Structure *structure);
	//////////////////////////////////////////////////////
	//midX���� ���� ��, �ڽĵ� x,width ����
	virtual void Accept1(Visitor *visitor, Long x, Long midX, Long previousX){}
	//contents ����
	virtual void CorrectContents(string object){}
	//accept
	virtual void Accept(Visitor *visitor, NSChart *nsChart, Structure *structure){}
	Block& operator = (const Block& source);
	Long GetMidX() const;
	Long GetMidY() const;
	Long GetCapacity() const;
	Long GetLength() const;

	virtual void Accept(Visitor *visitor, Long maxWidth, NSChart *nsChart){}
	virtual void AutoCorrect(Long x, Long y, Long width, Long height){}

protected:

	Long midX;
	Long midY;
	Long capacity;
	Long length;

private:

	Array<Structure*> structures;

};

inline Long Block::GetMidX() const {
	return this->midX;
}

inline Long Block::GetMidY() const {
	return this->midY;
}

inline Long Block::GetCapacity() const {
	return this->capacity;
}

inline Long Block::GetLength() const {
	return this->length;
}

int CompareStructures(void *one, void *other);
int CompareCoordinate(void *one, void *other);
#endif //_BLOCK_H