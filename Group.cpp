//Group.cpp
#include "Group.h"
#include "Structure.h"
#include "SelectedStructure.h"
Group::Group(){
	this->capacity = 100;
	this->length = 0;
	
}

Group::Group(SelectedStructure *selectedStructure){
	Long i = 0;
	Long index;
	Structure *structure;

	this->capacity = 100;
	this->length = 0;

	while (i < selectedStructure->GetLength()){
		structure = selectedStructure->GetAt(i);

		if (this->length < this->capacity){
			index = this->groupedStructures.Store(this->length, structure);
		}
		else if (this->length >= this->capacity){
			index = this->groupedStructures.AppendFromRear(structure);
			this->capacity += 1;
		}
		this->length += 1;

		i++;
	}
}

Group::Group(const Group& source)
	:groupedStructures(source.groupedStructures){
	this->capacity = source.capacity;
	this->length = source.length;
}

Group::~Group(){
	
}

Long Group::Add(Structure *structure){
	Long index;

	if (this->length < this->capacity){
		index = this->groupedStructures.Store(this->length, structure);
	}
	else if (this->length >= this->capacity){
		index = this->groupedStructures.AppendFromRear(structure);
		this->capacity += 1;
	}
	this->length += 1;

	return index;
}

//그룹에서 몇번째인지 알려줌
Long Group::Find(Structure *structure){
	Long i = 0;
	Long index = -1;

	while (i < this->length && index != -1){
		if (structure == this->groupedStructures[i]){
			index = i;
		}
		i++;
	}

	return index;
}

void Group::Clear(){
	this->groupedStructures.Clear();
	this->length = this->groupedStructures.GetLength();
	this->capacity = this->groupedStructures.GetCapacity();
}

Long Group::Delete(Long index){
	Long i;

	i = this->groupedStructures.Delete(index);
	this->length = this->groupedStructures.GetLength();
	this->capacity = this->groupedStructures.GetCapacity();

	return i;
}

Long Group::Search(Structure *structure){
	Long index = -1;

	index = this->groupedStructures.LinearSearchUnique(structure, CompareGroupStructureAddresses);

	return index;
}

void Group::Arrange() {

	this->groupedStructures.SelectionSort(CompareGroupStructureCoordinate);
}

void Group::ReverseArrange(){
	this->groupedStructures.SelectionSort(CompareGroupStructureYXCoordinate);

}

Structure* Group::GetAt(Long index){
	return this->groupedStructures.GetAt(index);
}

Group& Group::operator=(const Group& source){
	this->groupedStructures = source.groupedStructures;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

int CompareGroupStructureAddresses(void *one, void *other) {
	int ret = -1;
	if (*(static_cast<Structure**>(one)) == other) {
		ret = 0;
	}
	return ret;
}

int CompareGroupStructureYXCoordinate(void *one, void *other){
	int ret = 0;

	if ((*(static_cast<Structure**>(one)))->GetY() < (*(static_cast<Structure**>(other)))->GetY()){
		ret = 1;
	}

	else if ((*(static_cast<Structure**>(one)))->GetY() == (*(static_cast<Structure**>(other)))->GetY()){
		if ((*(static_cast<Structure**>(one)))->GetY() < (*(static_cast<Structure**>(other)))->GetY()){
			ret = 1;
		}
		else {
			ret = -1;
		}
	}
	else if ((*(static_cast<Structure**>(one)))->GetY() > (*(static_cast<Structure**>(other)))->GetY()){
		ret = -1;
	}
	return ret;
}

int CompareGroupStructureCoordinate(void *one, void *other) {
	int ret = 0;

	if ((*(static_cast<Structure**>(one)))->GetY() > (*(static_cast<Structure**>(other)))->GetY()) {
		ret = 1;
	}

	else if ((*(static_cast<Structure**>(one)))->GetY() == (*(static_cast<Structure**>(other)))->GetY()) {
		if ((*(static_cast<Structure**>(one)))->GetY() > (*(static_cast<Structure**>(other)))->GetY()) {
			ret = 1;
		}
		else {
			ret = -1;
		}
	}
	else if ((*(static_cast<Structure**>(one)))->GetY() < (*(static_cast<Structure**>(other)))->GetY()) {
		ret = -1;
	}
	return ret;
}