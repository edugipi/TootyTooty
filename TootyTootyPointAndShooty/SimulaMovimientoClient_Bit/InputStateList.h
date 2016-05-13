#pragma once
#include "InputState.h"
#include <vector>
#include <iostream>

class InputStateList: private std::vector<InputState>
{
private:
	int counter;
public:
	InputStateList():counter(0) {}
	
	void Add(InputState& _inputState)
	{
		counter++;
		push_back(_inputState);
	}

	int GetCounter()
	{
		return counter;
	}

	int Size()
	{
		return size();
	}

	bool Acknowledge(int _id, int _ackPositionX, int _ackPositionY)
	{
		bool okAck = true;
		int posRemove = -1;
		
		for (int i = 0; i < size(); i++)
		{
			InputState inputState = this->at(i);
			if (_id == inputState.GetId())
			{
				posRemove = i;
				if (inputState.GetAbsolutePositionX() != _ackPositionX) {
					std::cout << ">>>>>>>>Corrijo de " << inputState.GetAbsolutePositionX() << " a " << _ackPositionX << std::endl;
					//erase(begin() + posRemove +1, begin()+size());
					okAck = false;

				} else if (inputState.GetAbsolutePositionY() != _ackPositionY) {
					std::cout << ">>>>>>>>Corrijo de " << inputState.GetAbsolutePositionY() << " a " << _ackPositionY << std::endl;
					//erase(begin() + posRemove +1, begin()+size());
					okAck = false;

				}
				
				break;
			}
		}

		//Doy por validada la posición que me han confirmado y todas las anteriores.
		if (posRemove > -1)
		{
			std::cout << "Borro de la 0 a la " << posRemove << std::endl;
			erase(begin(), begin() + posRemove + 1);
		}
		
		return okAck;
	}
};