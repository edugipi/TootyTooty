#pragma once
#include <vector>
#include "PlayerCommand.h"

class PlayerCommandList: private std::vector<PlayerCommand>
{
public:
	PlayerCommandList();
	void AddCommand(int _idPlayer, int _idMove, int _delta, std::vector<int> _aDetailedPath);
	bool PopCommand(PlayerCommand& _playerCommand);
	int Size();
	~PlayerCommandList();
};

