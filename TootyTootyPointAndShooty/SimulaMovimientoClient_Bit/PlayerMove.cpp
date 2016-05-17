#include "PlayerMove.h"


PlayerMove::PlayerMove():deltaX(0), deltaY(0), idSquare(0)
{
}

PlayerMove::PlayerMove(int _deltaX, int _deltaY, int _idSquare): deltaX(_deltaX), deltaY(_deltaY), idSquare(_idSquare)
{
}

PlayerMove::PlayerMove(const PlayerMove & _playerMove):PlayerMove(_playerMove.deltaX, _playerMove.deltaY, _playerMove.idSquare)
{

}

int PlayerMove::GetDeltaX()
{
	return deltaX;
}

int PlayerMove::GetDeltaY()
{
	return deltaY;
}

int PlayerMove::GetIdSquare()
{
	return idSquare;
}

void PlayerMove::SetDelta(int _deltaX, int _deltaY) {
	deltaX = _deltaX;
	deltaY = _deltaY;

}

void PlayerMove::SetIdSquare(int _idSquare)
{
	idSquare = _idSquare;
}

PlayerMove::~PlayerMove()
{
}
