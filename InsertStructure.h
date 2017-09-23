//InsertStructure.h
#ifndef _INSERTSTRUCTURE_H
#define _INSERTSTRUCTURE_H
#include "MouseLButtonAction.h"

typedef signed long int Long;
class Structure;

class InsertStructure :public MouseLButtonAction{

public:
	InsertStructure();
	InsertStructure(const InsertStructure& source);
	~InsertStructure();
	virtual void Insert(Structure *insertStructure, Structure *markStructure, Long checkingResult);
	virtual void InsertGroup(Group *insertGroup, Structure *markStructure, Long checkingResult);
	InsertStructure& operator=(const InsertStructure& source);

};
#endif //_INSERTSTRUCTURE_H