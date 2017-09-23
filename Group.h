//Group.h
#ifndef _GROUP_H
#define _GROUP_H
#include "Array.h"

typedef signed long int Long;

class Structure;
class SelectedStructure;
class Visitor;

class Group{
	
public:
	Group();
	Group(SelectedStructure *selectedStructure);
	Group(const Group& source);
	~Group();
	Long Add(Structure *structure);
	Long Find(Structure *structure);
	void Clear();
	Long Delete(Long index);
	Long Search(Structure *structure);
	void Arrange();
	void ReverseArrange();
	Structure* GetAt(Long index);
	Group& operator=(const Group& source);
	Long GetCapacity() const;
	Long GetLength() const;


private:
	Array<Structure*> groupedStructures;
	Long capacity;
	Long length;

};

inline Long Group::GetCapacity() const{
	return this->capacity;
}

inline Long Group::GetLength() const{
	return this->length;
}

int CompareGroupStructureAddresses(void *one, void *other);
int CompareGroupStructureYXCoordinate(void *one, void *other);
int CompareGroupStructureCoordinate(void *one, void *other);
#endif //_GROUP_H