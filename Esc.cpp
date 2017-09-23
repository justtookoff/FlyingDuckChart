//Esc.cpp
#include "Esc.h"
#include "CopyStructure.h"

Esc::Esc(){

}

Esc::Esc(const Esc& source){

}

Esc::~Esc(){

}

void Esc::Clear(CopyStructure *copyStructure){

	copyStructure->Clear();
}

Esc& Esc::operator=(const Esc& source){

	return *this;
}