//MarkStructure.h
#ifndef _MARKSTRUCTURE_H
#define _MARKSTRUCTURE_H
#include "MouseLButtonAction.h"

class NSChart;
class Structure;
class GroupStructure;

class MarkStructure :public MouseLButtonAction{

public:
	MarkStructure();
	MarkStructure(const MarkStructure& source);
	~MarkStructure();
	virtual void Mark(NSChart *nsChart, GroupStructure *groupStructure, SelectedStructure *selectedStructure, Long x, Long y, Long *currentState, Long *previousState, Structure* *markStructure, Long *checkingResult);
	MarkStructure& operator=(const MarkStructure& source);

};
#endif //_MARKSTRUCTURE_H