//GroupIndexes.cpp

#include "GroupIndexes.h"

GroupIndexes::GroupIndexes() {
	this->length = 0;
	this->capacity = 100;
}

GroupIndexes::GroupIndexes(const GroupIndexes& source)
	:indexes(source.indexes), capacity(source.capacity), length(source.length) {

}

GroupIndexes::~GroupIndexes() {

}

GroupIndexes& GroupIndexes::operator=(const GroupIndexes& source) {
	this->indexes = source.indexes;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Long GroupIndexes::Add(Long groupIndex) {
	Long index;

	if (this->length < this->capacity) {
		index = this->indexes.Store(this->length, groupIndex);
	}

	else if (this->length >= this->capacity) {
		index = this->indexes.AppendFromRear(groupIndex);
		this->capacity += 1;
	}

	this->length += 1;

	return index;

}

Long& GroupIndexes::GetAt(Long index) {
	return this->indexes.GetAt(index);
}

void GroupIndexes::Clear() {
	this->indexes.Clear();
	this->length = this->indexes.GetLength();
	this->capacity = this->indexes.GetCapacity();
}

void GroupIndexes::Arrange() {
	this->indexes.SelectionSort(CompareIndexes);
}

int CompareIndexes(void *one, void *other) {
	int ret = 0;

	if (*((int*)one) < *((int*)other)) {
		ret = 1;
	}

	else if (*((int*)one) == *((int*)other)) {
		ret = 0;
	}

	else if (*((int*)one) > *((int*)other)) {
		ret = -1;
	}

	return ret;
}
