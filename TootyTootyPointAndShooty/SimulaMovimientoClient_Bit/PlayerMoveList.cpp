#include "PlayerMoveList.h"



PlayerMoveList::PlayerMoveList()
{
}

void PlayerMoveList::AddMove(int _deltaX, int _deltaY, int _idSquare)
{
	PlayerMove playerMove(_deltaX, _deltaY, _idSquare);
	push_back(playerMove);
}

void PlayerMoveList::AddMoves(int _idSquare, std::vector<int> _aDeltasX, std::vector<int> _aDeltasY) {
	if (_aDeltasX.size() > _aDeltasY.size()) {
		for (size_t i = _aDeltasY.size(); i < _aDeltasX.size(); i++)
		{
			_aDeltasY.push_back(0);
		}
	} else if (_aDeltasX.size() < _aDeltasY.size()) {
		for (size_t i = _aDeltasX.size(); i < _aDeltasY.size(); i++)
		{
			_aDeltasX.push_back(0);
		}
	}
	for (size_t i = 0; i < _aDeltasX.size(); i++)
	{
		AddMove(_aDeltasX[i], _aDeltasY[i], _idSquare);
	}

}

bool PlayerMoveList::PopMove(PlayerMove& _playerMove)
{
	if (size() == 0)
	{
		return false;
	}
	_playerMove = at(0);
	erase(begin(), begin() + 1);
	return true;
}


PlayerMoveList::~PlayerMoveList()
{
}
