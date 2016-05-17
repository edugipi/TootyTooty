#pragma once
class PlayerMove
{
private:
	int deltaX;
	int deltaY;
	int idSquare;
public:
	PlayerMove();
	PlayerMove(int _deltaX, int _deltaY, int _idSquare);
	PlayerMove(const PlayerMove& _playerMove);
	int GetDeltaX();
	int GetDeltaY();
	int GetIdSquare();
	void SetDelta(int _deltaX, int _deltaY);
	void SetIdSquare(int _idSquare);
	~PlayerMove();
};

