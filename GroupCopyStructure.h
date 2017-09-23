//GroupCopyStructure.h
#ifndef _GROUPCOPYSTRUCTURE_H
#define _GROUPCOPYSTRUCTURE_H
#include "Array.h"

typedef signed long int Long;

class Structure;
class Group;

class GroupCopyStructure{

public:
	GroupCopyStructure();
	GroupCopyStructure(const GroupCopyStructure& source);
	~GroupCopyStructure();
	Long Add(Group group);
	void Clear();
	Group& GetAt(Long index);
	GroupCopyStructure& operator=(const GroupCopyStructure& source);
	Long GetCapacity() const;
	Long GetLength() const;

private:
	Array<Group> groupCopyStructures;
	Long capacity;
	Long length;
};

inline Long GroupCopyStructure::GetCapacity() const{
	return this->capacity;
}

inline Long GroupCopyStructure::GetLength() const{
	return this->length;
}

#endif //_GROUPCOPYSTRUCTURE_H