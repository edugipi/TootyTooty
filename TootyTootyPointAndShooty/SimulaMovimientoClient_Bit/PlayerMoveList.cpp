#include "PlayerMoveList.h"



PlayerMoveList::PlayerMoveList()
{
}

void PlayerMoveList::AddMove(int _delta, int _idSquare)
{
	PlayerMove playerMove(_delta, _idSquare);
	push_back(playerMove);
}

void PlayerMoveList::AddMoves(int _idSquare, std::vector<int> _aDeltas)
{
	for (size_t i = 0; i < _aDeltas.size(); i++)
	{
		AddMove(_aDeltas[i], _idSquare);
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
