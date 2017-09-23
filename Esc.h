//Esc.h
#ifndef _ESC_H
#define _ESC_H
#include "KeyBoardAction.h"

class Esc :public KeyBoardAction{

public:
	Esc();
	Esc(const Esc& source);
	~Esc();
	virtual void Clear(CopyStructure *copyStructure);
	Esc& operator=(const Esc& source);

};
#endif //_ESC_H