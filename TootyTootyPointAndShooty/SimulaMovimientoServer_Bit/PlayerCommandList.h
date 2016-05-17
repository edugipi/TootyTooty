#pragma once
#include <vector>
#include "PlayerCommand.h"

class PlayerCommandList: private std::vector<PlayerCommand>
{
public:
	PlayerCommandList();
	void AddCommand(int _idPlayer, int _idMove, int _deltaX, int _deltaY, std::vector<int> _aDetailedPathX, std::vector<int> _aDetailedPathY);
	bool PopCommand(PlayerCommand& _playerCommand);
	int Size();
	~PlayerCommandList();
};

