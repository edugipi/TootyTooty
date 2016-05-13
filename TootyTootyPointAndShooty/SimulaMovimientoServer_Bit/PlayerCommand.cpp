#include "PlayerCommand.h"



PlayerCommand::PlayerCommand()
{
}

PlayerCommand::PlayerCommand(int _idPlayer, int _idMove, int _deltaX, int _deltaY, std::vector<int> _aDetailedPathX, std::vector<int> _aDetailedPathY): idPlayer(_idPlayer), idMove(_idMove),
	deltaX(_deltaX), deltaY(_deltaY), aStepsX(_aDetailedPathX), aStepsY(_aDetailedPathY)
{
}

PlayerCommand::PlayerCommand(const PlayerCommand & _playerCommand):
	PlayerCommand(_playerCommand.idPlayer, _playerCommand.idMove, _playerCommand.deltaX, _playerCommand.deltaY, _playerCommand.aStepsX, _playerCommand.aStepsY)
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

int PlayerCommand::GetDeltaX()
{
	return deltaX;
}

int PlayerCommand::GetDeltaY()
{
	return deltaY;
}

std::vector<int> PlayerCommand::GetDetailedPathX()
{
	return aStepsX;
}

std::vector<int> PlayerCommand::GetDetailedPathY()
{
	return aStepsY;
}


PlayerCommand::~PlayerCommand()
{
}
