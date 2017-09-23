//Block.cpp
#include "Block.h"
#include "Finder.h"
#include "SelectedStructure.h"
#include "CopyStructure.h"
#include "Arranger.h"

Block::Block(){
	this->length = 0;
	this->capacity = 100;
}
Block::Block(Long x, Long y, Long width, Long height, string contents, Long midX, Long midY)
	:Structure(x, y, width, height, contents){
	this->midX = midX;
	this->midY = midY;
	this->length = 0;
	this->capacity = 100;
}

Block::Block(const Block& source)
	:Structure(source), structures(source.structures){

	Long i = 0;

	while (i<source.length){
		this->structures.Modify(i, (const_cast<Block&>(source)).structures[i]->Clone());
		i++;
	}

	this->midX = source.midX;
	this->midY = source.midY;
	this->length = source.length;
	this->capacity = source.capacity;

}

Block& Block::operator=(const Block& source){
	Structure::operator=(source);

	Long i = 0;

	while (i < this->length){

		delete this->structures.GetAt(i);
		i++;
	}

	this->structures = source.structures;

	i = 0;

	while (i<source.length){
		this->structures.Modify(i, (const_cast<Block&>(source)).structures[i]->Clone());
		i++;
	}

	this->midX = source.midX;
	this->midY = source.midY;
	this->length = source.length;
	this->capacity = source.capacity;

	return *this;
}

Block::~Block(){
	Long i = 0;

	while (i < this->length){
		delete this->structures[i];
		i++;
	}
}

Long Block::Add(Structure *structure){
	Long index;

	if (this->length < this->capacity){
		index = this->structures.Store(this->length, structure);
	}

	else if (this->length >= this->capacity){
		index = this->structures.AppendFromRear(structure);
		this->capacity += 1;
	}

	this->length += 1;

	return index;
}

Structure* Block::Find(Long x, Long y){
	Structure *structure;
	Structure* indexStructure = 0;
	Visitor *visitor;
	visitor = new Finder;
	Long i = 0;
	
	while (i < this->GetLength()){
		structure = this->structures[i];
		structure->Accept(visitor, &indexStructure, x, y);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}

	return indexStructure;
}

void Block::DragFind(GroupStructure *groupStructure, GroupSelectedStructure *groupSelectedStructure, SelectedStructure *selectedStructure, Long previousX, Long previousY, Long currentX, Long currentY){
	Visitor *visitor = new Finder;
	Structure *structure;
	Long i = 0;

	while (i < this->GetLength()){
		structure = this->structures[i];
		structure->Accept(visitor, groupStructure, groupSelectedStructure, selectedStructure, previousX, previousY, currentX, currentY);
		i++;
	}

	if (visitor != 0) {
		delete visitor;
	}
}

void Block::Arrange(){
	Long i = 0;
	Structure *structure = 0;
	Visitor *visitor = new Arranger;
	this->structures.SelectionSort(CompareCoordinate);

	while (i < this->GetLength()){
		structure = this->GetChild(i);
		structure->Accept(visitor);
		i++;
	}

	if (visitor != 0){
		delete visitor;
	}

}

void Block::ArrangeChild(){
	this->structures.SelectionSort(CompareCoordinate);
}

/*
Long Block::Correct(Long index, string contents){
	//Structure *structure;
	//structure = this->structures[index]->Clone();
	//structure->GetContents() = contents; //칠판에 ->contents라 되어 있어서 변경함
	//this->structures.Modify(index, structure);
	//return index;
}
*/

Structure* Block::GetChild(Long index){
	return this->structures.GetAt(index);
}

Structure* Block::Clone(){
	return new Block(*this);
}

bool Block::IsEqual(const Block& other) {
	bool ret = false;
	if (Structure::operator==(other) == true &&
		this->midX == other.midX &&
		this->midY == other.midY &&
		this->capacity == other.capacity &&
		this->length == other.length) {
		ret = true;
	}
	return ret;
}

bool Block::IsNotEqual(const Block& other) {
	bool ret = false;
	if (Structure::operator!=(other) == true ||
		this->midX != other.midX ||
		this->midY != other.midY ||
		this->capacity != other.capacity ||
		this->length != other.length) {
		ret = true;
	}
	return ret;
}

bool Block::operator==(const Block& other) {
	bool ret = false;
	if (Structure::operator==(other) == true &&
		this->midX == other.midX &&
		this->midY == other.midY &&
		this->capacity == other.capacity &&
		this->length == other.length) {
		ret = true;
	}
	return ret;
}

bool Block::operator!=(const Block& other) {
	bool ret = false;
	if (Structure::operator!=(other) == true ||
		this->midX != other.midX ||
		this->midY != other.midY ||
		this->capacity != other.capacity ||
		this->length != other.length) {
		ret = true;
	}
	return ret;
}

Long Block::Search(Structure *structure){
	Long index = -1;

	index = this->structures.LinearSearchUnique(structure, CompareStructures);

	return index;
}

Long Block::Delete(Long index){
	Long i;

	i = this->structures.Delete(index);
	this->capacity = this->structures.GetCapacity();
	this->length = this->structures.GetLength();

	return i;

}

Long Block::Insert(Long index, Structure *structure){

	return this->structures.Insert(index, structure);
}

int CompareStructures(void *one, void *other){
	int ret = -1;
	if (*(static_cast<Structure**>(one)) == other) {
		ret = 0;
	}
	return ret;
	/*
	int ret = -1;

	if (one == other){
		ret = 0;
	}

	return ret;
	*/
}

int CompareCoordinate(void *one, void *other){
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

