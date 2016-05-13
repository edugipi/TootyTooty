#pragma once
#include <vector>
class PlayerCommand
{
private:
	int idPlayer;
	int idMove;
	int deltaX, deltaY;
	std::vector<int> aStepsX;
	std::vector<int> aStepsY;

public:
	PlayerCommand();
	PlayerCommand(int _idPlayer, int _idMove, int _deltaX, int _deltaY, std::vector<int> _aDetailedPathX, std::vector<int> _aDetailedPathY);
	PlayerCommand(const PlayerCommand& _playerCommand);
	int GetIdPlayer();
	int GetIdMove();
	int GetDeltaX();
	int GetDeltaY();
	std::vector<int> GetDetailedPathX();
	std::vector<int> GetDetailedPathY();
	~PlayerCommand();
};

