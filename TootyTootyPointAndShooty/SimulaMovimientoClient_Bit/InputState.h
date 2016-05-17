#pragma once
#include <string>
#include <vector>
#include "GameClientConstants.h"


class InputState
{
private:
	int absPosX, absPosY;
	std::vector<int> aMovesX;
	std::vector<int> aMovesY;
	int id;

public:
	InputState():absPosX(0),absPosY(0),id(0) {
		
	}

	InputState(const InputState& _inputState) {
		aMovesX = _inputState.aMovesX;
		aMovesY = _inputState.aMovesY;
		id = _inputState.id;
		absPosX = _inputState.absPosX;
		absPosY = _inputState.absPosY;

	}

	int GetId() {
		return id;

	}

	void SetId(int _id) {
		id = _id;

	}

	int GetDeltaX() {
		int delta = 0;
		for (size_t i = 0; i < aMovesX.size(); i++) {
			delta += aMovesX[i];

		} return delta;

	}

	int GetDeltaY() {
		int delta = 0;
		for (size_t i = 0; i < aMovesY.size(); i++) {
			delta += aMovesY[i];

		} return delta;

	}

	int GetAbsolutePositionX() {
		return absPosX;

	}

	int GetAbsolutePositionY() {
		return absPosY;

	}

	void SetAbsolutePosition(int _absPosX, int _absPosY) {
		absPosX = _absPosX;
		absPosY = _absPosY;

	}

	void AddLeft() {
		aMovesX.push_back(-1);

	}

	void AddRight() {
		aMovesX.push_back(1);

	}

	void AddUp() {
		aMovesY.push_back(-1);

	}

	void AddDown() {
		aMovesY.push_back(1);

	}

	std::vector<int> GetMovesX() {
		return aMovesX;

	}

	std::vector<int> GetMovesY() {
		return aMovesY;

	}

	void ResetMove() {
		aMovesX.clear();
		aMovesY.clear();

	}

	~InputState() {}
};

