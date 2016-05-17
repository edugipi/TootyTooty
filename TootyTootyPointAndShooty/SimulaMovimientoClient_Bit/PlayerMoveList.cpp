#include "PlayerMoveList.h"



PlayerMoveList::PlayerMoveList()
{
}

void PlayerMoveList::AddMove(int _deltaX, int _deltaY, int _idSquare)
{
	PlayerMove playerMove(_deltaX, _deltaY, _idSquare);
	push_back(playerMove);
}

void PlayerMoveList::AddMovesX(int _idSquare, std::vector<int> _aDeltas)
{
	for (size_t i = 0; i < _aDeltas.size(); i++)
	{
		AddMove(_aDeltas[i], 0, _idSquare);
	}
}

void PlayerMoveList::AddMovesY(int _idSquare, std::vector<int> _aDeltas)
{
	for (size_t i = 0; i < _aDeltas.size(); i++)
	{
		AddMove(0, _aDeltas[i], _idSquare);
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
