#pragma once
#include <vector>
#include "PlayerMove.h"

class PlayerMoveList : public std::vector<PlayerMove>
{
public:
	PlayerMoveList();
	void AddMove(int _deltaX, int _deltaY, int _idSquare);
	void AddMovesX(int _idSquare, std::vector<int> _aDeltas);
	void AddMovesY(int _idSquare, std::vector<int> _aDeltas);
	bool PopMove(PlayerMove& _playerMove);
	~PlayerMoveList();
};

