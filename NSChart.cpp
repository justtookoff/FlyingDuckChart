//NSChart.cpp
#include "NSChart.h"

NSChart::NSChart() {

}

NSChart::NSChart(const NSChart& source)
	:Block(source) {

}

NSChart::~NSChart() {

}

Structure* NSChart::Clone() {
	return new NSChart(*this);
}

Structure* NSChart::GetChild(Long index) {
	return Block::GetChild(index);
}

NSChart& NSChart::operator=(const NSChart& source) {
	Block::operator=(source);
	return *this;
}