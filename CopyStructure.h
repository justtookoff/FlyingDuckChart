//CopyStructure.h
#ifndef _COPYSTRUCTURE_H
#define _COPYSTRUCTURE_H
#include "Array.h"

typedef signed long int Long;

class Structure;

class CopyStructure{

public:
	CopyStructure();
	CopyStructure(const CopyStructure& source);
	~CopyStructure();
	Long Add(Structure *structure);
	void Clear();
	void Arrange();
	Structure* GetAt(Long index);
	CopyStructure& operator=(const CopyStructure& source);
	Long GetCapacity() const;
	Long GetLength() const;

private:
	Array<Structure*> copyStructures;
	Long capacity;
	Long length;
};

inline Long CopyStructure::GetCapacity() const{
	return this->capacity;
}

inline Long CopyStructure::GetLength() const{
	return this->length;
}

int CompareXYCoordinate(void *one, void *other);

#endif //_COPYSTRUCTURE_H