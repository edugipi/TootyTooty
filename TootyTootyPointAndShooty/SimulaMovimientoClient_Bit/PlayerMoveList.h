#pragma once
#include <vector>
#include "PlayerMove.h"

class PlayerMoveList : public std::vector<PlayerMove>
{
public:
	PlayerMoveList();
	void AddMove(int _deltaX, int _deltaY, int _idSquare);
	void AddMoves(int _idSquare, std::vector<int> _aDeltasX, std::vector<int> _aDeltasY);
	bool PopMove(PlayerMove& _playerMove);
	~PlayerMoveList();
};

