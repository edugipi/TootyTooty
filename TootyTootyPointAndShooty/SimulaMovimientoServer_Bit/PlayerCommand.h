#pragma once
#include <vector>
class PlayerCommand
{
private:
	int idPlayer;
	int idMove;
	int delta;
	std::vector<int> aSteps;

public:
	PlayerCommand();
	PlayerCommand(int _idPlayer, int _idMove, int _delta, std::vector<int> _aDetailedPath);
	PlayerCommand(const PlayerCommand& _playerCommand);
	int GetIdPlayer();
	int GetIdMove();
	int GetDelta();
	std::vector<int> GetDetailedPath();
	~PlayerCommand();
};

