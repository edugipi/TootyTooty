#include "PlayerCommandList.h"
#include <LittleSquare.h>
#include <iostream>


PlayerCommandList::PlayerCommandList()
{
}

/**
 * _str es el contenido del Dispatch_Message
 */
void PlayerCommandList::AddCommand(int _idPlayer, int _idMove, int _deltaX, int _deltaY, std::vector<int> _aDetailedPathX, std::vector<int> _aDetailedPathY)
{
	PlayerCommand playerCommand(_idPlayer, _idMove, _deltaX, _deltaY, _aDetailedPathX, _aDetailedPathY);
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
