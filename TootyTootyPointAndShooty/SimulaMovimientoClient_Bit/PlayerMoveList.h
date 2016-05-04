#pragma once
#include <vector>
#include "PlayerMove.h"

class PlayerMoveList : public std::vector<PlayerMove>
{
public:
	PlayerMoveList();
	void AddMove(int _delta, int _idSquare);
	void AddMoves(int _idSquare, std::vector<int> _aDeltas);
	bool PopMove(PlayerMove& _playerMove);
	~PlayerMoveList();
};

