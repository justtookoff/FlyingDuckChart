//GroupSelectedStructure.cpp
#include "GroupSelectedStructure.h"

GroupSelectedStructure::GroupSelectedStructure(){
	this->capacity = 100;
	this->length = 0;
}

GroupSelectedStructure::GroupSelectedStructure(const GroupSelectedStructure& source)
	: groupArray(source.groupArray){
		this->capacity = source.capacity;
		this->length = source.length;
}

GroupSelectedStructure::~GroupSelectedStructure(){

}

Long GroupSelectedStructure::Add(Group *group){
	Long index = -1;
	Long i = 0;

	index = this->groupArray.LinearSearchUnique(group, CompareGroupSelectedAddresses);
	
	if(index == -1){
		if(this->length < this->capacity){
			index = this->groupArray.Store(this->length, group);
		}
		if(this->length >= this->capacity){
			index = this->groupArray.AppendFromRear(group);
			this->capacity++;
		}
		this->length++;
	}
	return index;
}

void GroupSelectedStructure::Clear(){
	this->groupArray.Clear();
	this->length = this->groupArray.GetLength();
	this->capacity = this->groupArray.GetCapacity();
}

Long GroupSelectedStructure::Delete(Long index){
	Long i;

	i = this->groupArray.Delete(index);
	this->length = this->groupArray.GetLength();
	this->capacity = this->groupArray.GetCapacity();

	return i;
}

Long GroupSelectedStructure::Search(Group *group){
	Long index = -1;

	index = this->groupArray.LinearSearchUnique(group, CompareGroupSelectedAddresses);
	
	return index;
}

Group* GroupSelectedStructure::GetAt(Long index){
	return this->groupArray.GetAt(index);
}

GroupSelectedStructure& GroupSelectedStructure::operator=(const GroupSelectedStructure& source){
	this->groupArray = source.groupArray;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

int CompareGroupSelectedAddresses(void *one, void *other){
	int ret = -1;
	if(*(static_cast<Group**>(one)) == other){
		ret = 0;
	}
	return ret;
}
