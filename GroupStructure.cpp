//GroupStructure.cpp
#include "GroupStructure.h"
#include "SelectedStructure.h"

GroupStructure::GroupStructure(){
	this->capacity = 100;
	this->length = 0;
}

GroupStructure::GroupStructure(const GroupStructure& source)
	:groups(source.groups){
	this->capacity = source.capacity;
	this->length = source.length;
}

GroupStructure::~GroupStructure(){

}

Long GroupStructure::Add(SelectedStructure *selectedStructure){
	Long index;

	Group group(selectedStructure);

	if (this->length < this->capacity){
		index = this->groups.Store(this->length, group);
	}
	else if (this->length >= this->capacity){
		index = this->groups.AppendFromRear(group);
		this->capacity += 1;
	}
	this->length += 1;

	return index;
}

Long GroupStructure::Add(Group group) {
	Long index;

	if (this->length < this->capacity) {
		index = this->groups.Store(this->length, group);
	}
	else if (this->length >= this->capacity) {
		index = this->groups.AppendFromRear(group);
		this->capacity += 1;
	}
	this->length += 1;

	return index;
}

void GroupStructure::Clear(){
	this->groups.Clear();
	this->length = this->groups.GetLength();
	this->capacity = this->groups.GetCapacity();
}

Long GroupStructure::Delete(Long index){
	Long i;

	i = this->groups.Delete(index);
	this->length = this->groups.GetLength();
	this->capacity = this->groups.GetCapacity();

	return i;
}

Long GroupStructure::Search(Group *group){
	Long index = -1;

	index = this->groups.LinearSearchUnique(group, CompareGroupAddresses);

	return index;
}



Group& GroupStructure::GetAt(Long index){
	return this->groups.GetAt(index);
}

GroupStructure& GroupStructure::operator=(const GroupStructure& source){
	this->groups = source.groups;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

int CompareGroupAddresses(void *one, void *other) {
	int ret = -1;
	if ((static_cast<Group**>(one)) == other) {
		ret = 0;
	}
	return ret;
}


