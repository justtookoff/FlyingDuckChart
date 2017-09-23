//NSChart.h
#ifndef _NSCHART_H
#define _NSCHART_H
#include "Block.h"

class NSChart :public Block{
public:
	NSChart();
	NSChart(const NSChart& source);
	virtual ~NSChart();
	virtual Structure* Clone();
	virtual Structure* GetChild(Long index);
	NSChart& operator=(const NSChart& source);
};

#endif //_NSCHART_H