#pragma once
#include "GameConstants.h"
#include <string>
#include <vector>

class LittleSquare {
protected:
	int pos_x;
	int pos_y;

public:
	LittleSquare();
	LittleSquare(int _pos_x, int _pos_y);
	LittleSquare(const LittleSquare& littleSquare);
	int GetPositionX();
	int GetPositionY();
	void SetPosition(int _pos_x, int _pos_y);
	void AddRight();
	void AddLeft();
	void AddUp();
	void AddDown();
	~LittleSquare();
	static int CalculateDeltaMovement(std::string _str);
	static int CalculateDeltaMovement(std::vector<int> _aPositions);

};
