//CheckingPoint.h
#ifndef _CHECKINGPOINT_H
#define _CHECKINGPOINT_H

typedef signed long int Long;

class SelectedStructure;
class Structure;
class Selection;
class CheckingPoint {

public:
	CheckingPoint();
	CheckingPoint(const CheckingPoint& source);
	~CheckingPoint();
	Long CheckingResult(Structure *structure, Long x, Long y);
	Long CheckingMarkStructure(Structure *structure, Long x, Long y);
	CheckingPoint& operator=(const CheckingPoint& source);
	
};

#endif //_CHECKINGPOINT_H