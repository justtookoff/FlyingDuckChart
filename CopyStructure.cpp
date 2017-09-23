//CopyStructure.cpp
#include "CopyStructure.h"
#include "Structure.h"

CopyStructure::CopyStructure(){
	this->length = 0;
	this->capacity = 100;
}

CopyStructure::CopyStructure(const CopyStructure& source)
	:copyStructures(source.copyStructures){
	this->capacity = source.capacity;
	this->length = source.length;
}

CopyStructure::~CopyStructure(){
	Long i = 0;

	while (i < this->copyStructures.GetLength()){
		delete this->copyStructures.GetAt(i);

		i++;
	}
}

Long CopyStructure::Add(Structure *structure){
	Long index;

	if (this->length < this->capacity){
		index = this->copyStructures.Store(this->length, structure);
	}
	else if (this->length >= this->capacity){
		index = this->copyStructures.AppendFromRear(structure);
		this->capacity += 1;
	}
	this->length += 1;

	return index;
}

void CopyStructure::Clear(){
	Long i = 0;

	while (i < this->length){
		delete this->copyStructures.GetAt(i);
		i++;
	}

	this->copyStructures.Clear();
	this->capacity = this->copyStructures.GetCapacity();
	this->length = this->copyStructures.GetLength();
}

void CopyStructure::Arrange(){
	this->copyStructures.SelectionSort(CompareXYCoordinate);
}

Structure* CopyStructure::GetAt(Long index){
	return this->copyStructures.GetAt(index);
}

CopyStructure& CopyStructure::operator=(const CopyStructure& source){
	this->copyStructures = source.copyStructures;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

int CompareXYCoordinate(void *one, void *other){
	int ret = 0;

	if ((*(static_cast<Structure**>(one)))->GetY() > (*(static_cast<Structure**>(other)))->GetY()){
		ret = 1;
	}

	else if ((*(static_cast<Structure**>(one)))->GetY() == (*(static_cast<Structure**>(other)))->GetY()){
		if ((*(static_cast<Structure**>(one)))->GetY() > (*(static_cast<Structure**>(other)))->GetY()){
			ret = 1;
		}
		else {
			ret = -1;
		}
	}
	else if ((*(static_cast<Structure**>(one)))->GetY() < (*(static_cast<Structure**>(other)))->GetY()){
		ret = -1;
	}
	return ret;
}