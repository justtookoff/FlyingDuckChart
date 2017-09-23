//AutoCollectorGetMaxWidth.h

#ifndef _AUTOCOLLECTORGETMAXWIDTH_H
#define _AUTOCOLLECTORGETMAXWIDTH_H

typedef signed long int Long;

class SelectedStructure;
class NSChart;
class AutoCollectorGetMaxWidth{

public:
	AutoCollectorGetMaxWidth();
	AutoCollectorGetMaxWidth(const AutoCollectorGetMaxWidth& source);
	~AutoCollectorGetMaxWidth();
	AutoCollectorGetMaxWidth& operator=(const AutoCollectorGetMaxWidth& source);

	void GetMaxWidth(SelectedStructure *selectedStructure, NSChart *nsChart);

};

#endif //_AUTOCOLLECTORGETMAXWIDTH_H