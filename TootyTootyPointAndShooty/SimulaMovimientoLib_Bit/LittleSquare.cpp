#include "LittleSquare.h"
#pragma once
#include "LittleSquare.h"
#include <iostream>


LittleSquare::LittleSquare()
{
}

LittleSquare::LittleSquare(int _pos_x, int _pos_y) : pos_x(_pos_x), pos_y(_pos_y) {

}

LittleSquare::LittleSquare(const LittleSquare& littleSquare) : LittleSquare(littleSquare.pos_x, littleSquare.pos_y) {
	
}

int LittleSquare::GetPositionX() { 
	return pos_x;

}

int LittleSquare::GetPositionY() {
	return pos_y;

}

void LittleSquare::SetPosition(int _pos_x, int _pos_y) { 
	pos_x = _pos_x;
	pos_y = _pos_y;

}

void LittleSquare::AddRight() {
	pos_x++;
	//std::cout << "Cuadrado en: " << position << std::endl;

}

void LittleSquare::AddLeft() {
	pos_x--;
	//std::cout << "Cuadrado en: " << position << std::endl;

}

void LittleSquare::AddUp() {
	pos_y--;

}

void LittleSquare::AddDown() {
	pos_y++;

}

LittleSquare::~LittleSquare() {

}

int LittleSquare::CalculateDeltaMovement(std::string _str) {
	int index2Ptos = _str.find_first_of(':');
	int delta = 0;
	if (index2Ptos == std::string::npos) {
		delta += atoi(_str.c_str());

	} while (index2Ptos != std::string::npos) {
		std::string strPosition = _str.substr(0, index2Ptos);
		std::string strTheOthers = _str.substr(index2Ptos + 1, _str.size() - index2Ptos);

		delta += atoi(strPosition.c_str());

		index2Ptos = strTheOthers.find_first_of(':');
		if (index2Ptos == std::string::npos) {
			delta += atoi(strTheOthers.c_str());

		} _str = strTheOthers;

	} return delta;

}

int LittleSquare::CalculateDeltaMovement(std::vector<int> _aPositions) {
	int delta = 0;
	for (size_t i = 0; i < _aPositions.size(); i++) {
		delta += _aPositions[i];

	} return delta;

}