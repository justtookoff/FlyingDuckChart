//GroupStructure.h
#ifndef _GROUPSTRUCTURE_H
#define _GROUPSTRUCTRUE_H
#include "Array.h"
#include "Group.h"

typedef signed long int Long;

class Structure;
class SelectedStructure;

class GroupStructure {

public:
	GroupStructure();
	GroupStructure(const GroupStructure& source);
	~GroupStructure();
	Long Add(SelectedStructure *selectedStructure);
	Long Add(Group group);
	void Clear();
	Long Delete(Long index);
	Long Search(Group *group);
	Group& GetAt(Long index);
	GroupStructure& operator=(const GroupStructure& source);
	Long GetCapacity() const;
	Long GetLength() const;

private:
	Array<Group> groups;
	Long capacity;
	Long length;
};

inline Long GroupStructure::GetCapacity() const{
	return this->capacity;
}

inline Long GroupStructure::GetLength() const{
	return this->length;
}

int CompareGroupAddresses(void *one, void *other);
#endif //_GROUPSTRUCTURE_H