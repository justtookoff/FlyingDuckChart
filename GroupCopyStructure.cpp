//GroupCopyStructure.cpp
#include "GroupCopyStructure.h"
#include "Group.h"
#include "Structure.h"
#include "Block.h"

GroupCopyStructure::GroupCopyStructure(){
	this->length  = 0;
	this->capacity = 100;
}

GroupCopyStructure::GroupCopyStructure(const GroupCopyStructure& source)
	: groupCopyStructures(source.groupCopyStructures){
		this->capacity = source.capacity;
		this->length = source.length;
}

GroupCopyStructure::~GroupCopyStructure(){
	Long i = 0;
	Long v = 0;
	while (i < this->length) {
		Long j = 0;
		Group group = this->groupCopyStructures.GetAt(i);
		while (j < group.GetLength()) {
			Structure *structure = group.GetAt(j);

			if (dynamic_cast<Block *>(structure)) {
				while (v != dynamic_cast<Block *>(structure)->GetLength()) {
					dynamic_cast<Block *>(structure)->Delete(v);
				}
			}
			delete structure;
			j++;
		}
		i++;
	}
}

Long GroupCopyStructure::Add(Group group){
	Long index;
		
	if(this->length < this->capacity){
		index = this->groupCopyStructures.Store(this->length, group);
	}
	else if(this->length >= this->capacity){
		index = this->groupCopyStructures.AppendFromRear(group);
		this->capacity += 1;
	}
	this->length += 1;

	return index;
}

void GroupCopyStructure::Clear(){
	this->groupCopyStructures.Clear();
	this->capacity = this->groupCopyStructures.GetCapacity();
	this->length = this->groupCopyStructures.GetLength();
}

Group& GroupCopyStructure::GetAt(Long index){
	return this->groupCopyStructures.GetAt(index);
}

GroupCopyStructure& GroupCopyStructure::operator=(const GroupCopyStructure& source){
	this->groupCopyStructures = source.groupCopyStructures;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

