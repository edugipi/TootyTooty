#include "PlayerMove.h"


PlayerMove::PlayerMove():delta(0),idSquare(0)
{
}

PlayerMove::PlayerMove(int _delta, int _idSquare): delta(_delta), idSquare(_idSquare)
{
}

PlayerMove::PlayerMove(const PlayerMove & _playerMove):PlayerMove(_playerMove.delta, _playerMove.idSquare)
{

}

int PlayerMove::GetDelta()
{
	return delta;
}

int PlayerMove::GetIdSquare()
{
	return idSquare;
}

void PlayerMove::SetDelta(int _delta)
{
	delta = _delta;
}

void PlayerMove::SetIdSquare(int _idSquare)
{
	idSquare = _idSquare;
}

PlayerMove::~PlayerMove()
{
}
