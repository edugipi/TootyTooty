#pragma once
#include "LittleSquare.h"
#include <iostream>


LittleSquare::LittleSquare(int _position) :position(_position) 
{
}

LittleSquare::LittleSquare(const LittleSquare& littleSquare) : LittleSquare(littleSquare.position)
{
	
}

int LittleSquare::GetPosition() 
{ 
	return position; 
}

void LittleSquare::SetPosition(int _position) 
{ 
	position = _position; 
}

void LittleSquare::AddRight()
{
	position++;
	//std::cout << "Cuadrado en: " << position << std::endl;
}

void LittleSquare::AddLeft()
{
	position--;
	//std::cout << "Cuadrado en: " << position << std::endl;
}

LittleSquare::~LittleSquare() 
{
}

int LittleSquare::CalculateDeltaMovement(std::string _str)
{
	int index2Ptos = _str.find_first_of(':');
	int delta = 0;
	if (index2Ptos == std::string::npos)
	{
		delta += atoi(_str.c_str());
	}

	while (index2Ptos != std::string::npos)
	{
		std::string strPosition = _str.substr(0, index2Ptos);
		std::string strTheOthers = _str.substr(index2Ptos + 1, _str.size() - index2Ptos);

		delta += atoi(strPosition.c_str());

		index2Ptos = strTheOthers.find_first_of(':');
		if (index2Ptos == std::string::npos)
		{
			delta += atoi(strTheOthers.c_str());
		}
		_str = strTheOthers;
	}
	return delta;
}

int LittleSquare::CalculateDeltaMovement(std::vector<int> _aPositions)
{
	int delta = 0;
	for (size_t i = 0; i < _aPositions.size(); i++)
	{
		delta += _aPositions[i];
	}
	return delta;
}