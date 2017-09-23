//GroupIndexes.h

#ifndef _GROUPINDEXES_H
#define _GROUPINDEXES_H
#include "Array.h"

typedef signed long int Long;

class GroupIndexes {
public :
	GroupIndexes();
	GroupIndexes(const GroupIndexes& source);
	~GroupIndexes();
	GroupIndexes& operator=(const GroupIndexes& source);
	Long Add(Long groupIndex);
	Long& GetAt(Long index);
	void Clear();
	void Arrange();
	Long GetCapacity() const;
	Long GetLength() const;

private :
	Array<Long>indexes;
	Long capacity;
	Long length;
};

inline Long GroupIndexes::GetCapacity() const {
	return this->capacity;
}

inline Long GroupIndexes::GetLength() const {
	return this->length;
}

int CompareIndexes(void *one, void *other);
#endif // !_GROUPINDEXES_H
