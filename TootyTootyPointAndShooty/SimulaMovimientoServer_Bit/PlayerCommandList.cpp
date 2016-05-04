#include "PlayerCommandList.h"
#include <LittleSquare.h>
#include <iostream>


PlayerCommandList::PlayerCommandList()
{
}

/**
 * _str es el contenido del Dispatch_Message
 */
void PlayerCommandList::AddCommand(int _idPlayer, int _idMove, int _delta, std::vector<int> _aDetailedPath)
{
	PlayerCommand playerCommand(_idPlayer, _idMove, _delta, _aDetailedPath);
	push_back(playerCommand);
}

bool PlayerCommandList::PopCommand(PlayerCommand & _playerCommand)
{
	if (size() == 0)
	{
		return false;
	}
	_playerCommand = at(0);
	erase(begin(), begin() + 1);
	return true;
}

int PlayerCommandList::Size()
{
	return size();
}


PlayerCommandList::~PlayerCommandList()
{
}
