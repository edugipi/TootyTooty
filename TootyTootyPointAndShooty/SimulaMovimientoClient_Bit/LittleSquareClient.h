#include <LittleSquare.h>
#include <vector>
#include "PlayerMoveList.h"

class LittleSquareClient : public LittleSquare {
public:
	LittleSquareClient() :LittleSquare(330, 280) {}
	bool IsValidPosition();
	void SetDelta(int _deltaX, int _deltaY);
	static std::pair <int, int> CalculateEndPosition(int _curPosX, int _curPosY, PlayerMoveList & _aPlayersMovesX, PlayerMoveList & _aPlayersMovesY);
	static void InterpolatePath(int _start, int _end, std::string& _strPositions);
	static void InterpolatePath(int _start, int _end, std::vector<int>& _aPositions);
	static std::vector<int> CompressPath(int _numSteps, std::vector<int>& _aPositions);
	static std::vector<int> ParserListPositions(std::string _positions);

};