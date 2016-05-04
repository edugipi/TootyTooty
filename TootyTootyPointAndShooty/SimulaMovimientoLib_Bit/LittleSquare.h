#pragma once
#include "GameConstants.h"
#include <string>
#include <vector>

class LittleSquare
{
protected:
	int position;

public:
	
	LittleSquare(int _position);
	LittleSquare(const LittleSquare& littleSquare);
	int GetPosition();
	void SetPosition(int _position);
	void AddRight();
	void AddLeft();
	~LittleSquare();
	static int CalculateDeltaMovement(std::string _str);
	static int CalculateDeltaMovement(std::vector<int> _aPositions);
};
