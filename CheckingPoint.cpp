//CheckingPoint.cpp
#include "CheckingPoint.h"
#include "Structure.h"
#include "Selection.h"
#include "Iteration.h"
#include "Case.h"

CheckingPoint::CheckingPoint() {

}

CheckingPoint::CheckingPoint(const CheckingPoint& source) {

}

CheckingPoint::~CheckingPoint() {

}

Long CheckingPoint::CheckingResult(Structure *structure, Long x, Long y) {
	Long result = -1;
	Long xPoint = structure->GetX();
	Long yPoint = structure->GetY();
	Long xDistance = structure->GetWidth() / 10;
	Long yDistance = structure->GetHeight() / 10;
	Long width = structure->GetWidth();
	Long height = structure->GetHeight();
	Long endX = structure->GetX() + structure->GetWidth();
	Long endY = structure->GetY() + structure->GetHeight();
	Long midX;
	Long midY;

	if (xPoint -5 <= x && xPoint + 5 >= x &&
		yPoint -5 <= y && yPoint+5  >= y){ 
		result = 0;
	}

	else if (xPoint + width/2 +5 >= x && xPoint + width/2  - 5 <= x &&
		yPoint -5 <= y && yPoint + 5 >= y) { 
		result = 4;
	}

	else if (xPoint + width -5  <= x && xPoint + width +5 >= x &&
		yPoint -5  <= y && yPoint +5 >= y){ 
		result = 1;
	}

	else if (xPoint +width -5 <= x && xPoint + width +5 >= x &&
		yPoint + height /2 -5 <= y && yPoint + height/2 +5 >= y) { 
		result = 5;
	}

	else if (xPoint + width -5 < x && xPoint + width +5 >= x && 
		yPoint+ height -5 <= y && yPoint + height +5 >= y) { ///2
		result = 2;
	} 

	else if (xPoint + width /2 -5  <= x && xPoint + width/2 +5 >= x &&
		yPoint + height -5 < y && yPoint + height +5 > y) { ///4
		result = 6;
	}

	else if (xPoint -5 <= x && xPoint +5 >= x && 
		yPoint + height - 5 <= y && yPoint + height + 5 >= y) { ///6
		result = 3;
	}

	else if (xPoint -5  <= x && xPoint + 5 >= x && 
		yPoint + height/2 -5 <= y && yPoint + height/2 +5 >= y) { ///8
		result = 7;
	}

	//반복문
	if (dynamic_cast<Iteration *>(structure)) {
		midX = dynamic_cast<Iteration *>(structure)->GetMidX();
		midY = dynamic_cast<Iteration *>(structure)->GetMidY();
		if (midX-5 <= x && midX+5 >= x &&
			midY -5 <= y && y <= midY +5){
			result = 8;
		}
	}

	//선택문
	if (dynamic_cast<Selection *>(structure)) {
		midX = dynamic_cast<Selection *>(structure)->GetMidX();
		midY = dynamic_cast<Selection *>(structure)->GetMidY();
		
		if (midX -5 <= x && x <= midX+5  && midY - 5<= y && y <= midY +5) {

			result = 9;
		}

	}

	//케이스
	if (dynamic_cast<Case *>(structure)) {
		midX = dynamic_cast<Case *>(structure)->GetMidX();
		midY = dynamic_cast<Case *>(structure)->GetMidY();
		
		if (midX - 5 <= x && x <= midX + 5 && midY - 5 <= y && y <= midY + 5) {

			result = 10;
		}
	}

	return result;
}

Long CheckingPoint::CheckingMarkStructure(Structure *structure, Long x, Long y) {
	Long result = -1;
	Long xPoint = structure->GetX();
	Long yPoint = structure->GetY();
	Long xDistance = structure->GetWidth() / 10;
	Long yDistance = structure->GetHeight() / 10;
	Long width = structure->GetWidth();
	Long height = structure->GetHeight();
	Long endX = structure->GetX() + structure->GetWidth();
	Long endY = structure->GetY() + structure->GetHeight();
	Long midX;
	Long midY;


	if (xPoint /*+ xDistance */< x && endX /*- xDistance*/ > x &&
		yPoint <= y && yPoint + yDistance >= y) {
		result = 4;
	}

	else if (xPoint /*+ xDistance */< x && endX /*- xDistance*/> x &&
		endY - yDistance <= y && endY >= y) {
		result = 6;
	}


	//반복문
	if (dynamic_cast<Iteration *>(structure)) {
		midX = dynamic_cast<Iteration *>(structure)->GetMidX();
		midY = dynamic_cast<Iteration *>(structure)->GetMidY();
		if (midX <= x && x <= endX &&
			midY >= y && y >= yPoint + yDistance){
			result = 8;
		}
	}

	//선택문
	if (dynamic_cast<Selection *>(structure)) {
		midX = dynamic_cast<Selection *>(structure)->GetMidX();
		midY = dynamic_cast<Selection *>(structure)->GetMidY();
		Long width = dynamic_cast<Selection *>(structure)->GetWidth();

		if (xPoint <= x && x < midX  && midY - (midY - yPoint) / 5 <= y && y < midY) {

			result = 9;
		}
		else if (midX <= x && x < xPoint + width && midY - (midY - yPoint) / 5 <= y && y < midY) {

			result = 10;
		}
	}

	//케이스
	if (dynamic_cast<Case *>(structure)) {

		Long count = dynamic_cast<Case *>(structure)->GetCount();
		Long i = 0;
		Long flag = 0;
		Long midY2 = dynamic_cast<Case *>(structure)->GetMidY() + 30;
		Long standardWidth = dynamic_cast<Case *>(structure)->GetMidX() - dynamic_cast<Case *>(structure)->GetX();
		Long divideWidth = standardWidth / (count + 1);
		Long currentX = dynamic_cast<Case *>(structure)->GetX();
		midX = dynamic_cast<Case *>(structure)->GetMidX();
		midY = dynamic_cast<Case *>(structure)->GetMidY();
		//Long flag2 = 0;

		if (midX <= x && x <= endX &&  midY - (midY - yPoint) / 5 <= y && y < midY) {
			result = 11;
		}

		else {
			while (i < count + 1 && flag != 1) {
				if (currentX <= x && x <= currentX + divideWidth && midY <= y && y < midY2) {
					flag++;
				}

				currentX += divideWidth;
				i++;
			}

			if (flag == 1) {
				result = 11 + i;
			}
		}
	}
	return result;
}


CheckingPoint& CheckingPoint::operator=(const CheckingPoint& source) {

	return *this;
}