//CheckReadySequence.h

#ifndef _CHECKREADYSEQUENCE_H
#define _CHECKREADYSEQUENCE_H
#include "Array.h"
#include <string>
using namespace std;
typedef signed long int Long;
class NSChart;
class Structure;
class CheckReadySequence{
public:

	CheckReadySequence();
	~CheckReadySequence();
	CheckReadySequence(const CheckReadySequence& source);
	CheckReadySequence& operator =(const CheckReadySequence& source);

	void GetReadySequence(NSChart *nsChart, Structure **readyStructure);
	void ReadySequenceStrings(Structure *readyStructure,char input, string*indexes, Long *count,Long *sameWordIndexes,Long *sameWordCount);
	

private:
	

};

#endif _CHECKREADYSEQUENCE_H
