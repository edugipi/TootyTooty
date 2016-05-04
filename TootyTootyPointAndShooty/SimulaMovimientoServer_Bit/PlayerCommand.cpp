#include "PlayerCommand.h"



PlayerCommand::PlayerCommand()
{
}

PlayerCommand::PlayerCommand(int _idPlayer, int _idMove, int _delta, std::vector<int> _aDetailedPath): idPlayer(_idPlayer), idMove(_idMove),
	delta(_delta), aSteps(_aDetailedPath)
{
}

PlayerCommand::PlayerCommand(const PlayerCommand & _playerCommand):
	PlayerCommand(_playerCommand.idPlayer, _playerCommand.idMove, _playerCommand.delta, _playerCommand.aSteps)
{
}

int PlayerCommand::GetIdPlayer()
{
	return idPlayer;
}

int PlayerCommand::GetIdMove()
{
	return idMove;
}

int PlayerCommand::GetDelta()
{
	return delta;
}

std::vector<int> PlayerCommand::GetDetailedPath()
{
	return aSteps;
}


PlayerCommand::~PlayerCommand()
{
}
