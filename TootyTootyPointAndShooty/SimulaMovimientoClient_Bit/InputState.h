#pragma once
#include <string>
#include <vector>
#include "GameClientConstants.h"


class InputState
{
private:
	int absolutePosition;
	std::vector<int> aMoves;
	int id;

public:
	InputState():absolutePosition(0),id(0)
	{
		
	}

	InputState(const InputState& _inputState)
	{
		aMoves = _inputState.aMoves;
		id = _inputState.id;
		absolutePosition = _inputState.absolutePosition;
	}

	int GetId()
	{
		return id;
	}

	void SetId(int _id)
	{
		id = _id;
	}

	int GetDelta()
	{
		int delta = 0;
		for (size_t i = 0; i < aMoves.size(); i++)
		{
			delta += aMoves[i];
		}
		return delta;
	}

	int GetAbsolutePosition()
	{
		return absolutePosition;
	}

	void SetAbsolutePosition(int _absolutePosition)
	{
		absolutePosition = _absolutePosition;
	}

	void AddLeft()
	{
		aMoves.push_back(-1);
	}

	void AddRight()
	{
		aMoves.push_back(1);
	}

	std::vector<int> GetMoves()
	{
		return aMoves;
	}

	void ResetMove()
	{
		aMoves.clear();
	}

	~InputState() {}
};

