#pragma once
class PlayerMove
{
private:
	int delta;
	int idSquare;
public:
	PlayerMove();
	PlayerMove(int _delta, int _idSquare);
	PlayerMove(const PlayerMove& _playerMove);
	int GetDelta();
	int GetIdSquare();
	void SetDelta(int _delta);
	void SetIdSquare(int _idSquare);
	~PlayerMove();
};

