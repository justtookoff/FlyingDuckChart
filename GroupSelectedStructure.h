//GroupSelectedStructure.h
#ifndef _GROUPSELECTEDSTRUCTURE_H
#define _GROUPSELECTEDSTRUCTURE_H
#include "Array.h"

class Group;
class GroupSelectedStructure {
public:
	GroupSelectedStructure();
	GroupSelectedStructure(const GroupSelectedStructure& source);
	~GroupSelectedStructure();
	Long Add(Group *group);
	void Clear();
	Long Delete(Long index);
	Long Search(Group *group);
	Group* GetAt(Long index);
	GroupSelectedStructure& operator=(const GroupSelectedStructure& source);
	Long GetCapacity() const;
	Long GetLength() const;

private:
	Array<Group*> groupArray;
	Long capacity;
	Long length;
};

inline Long GroupSelectedStructure::GetCapacity() const{
	return this->capacity;
}

inline Long GroupSelectedStructure::GetLength() const{
	return this->length;
}

int CompareGroupSelectedAddresses(void *one, void *other);
	
#endif //_GROUPSELECTEDSTRUCTURE_H