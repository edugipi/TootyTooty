#include <LittleSquare.h>
#include <vector>
#include "PlayerMoveList.h"

class LittleSquareClient : public LittleSquare
{
public:
	LittleSquareClient() :LittleSquare(MIN_SQUARE) {}
	bool IsValidPosition();
	void SetDelta(int _delta);
	static int CalculateEndPosition(int _currentPosition, PlayerMoveList& _aPlayersMoves);
	static void InterpolatePath(int _start, int _end, std::string& _strPositions);
	static void InterpolatePath(int _start, int _end, std::vector<int>& _aPositions);
	static std::vector<int> CompressPath(int _numSteps, std::vector<int>& _aPositions);
	static std::vector<int> ParserListPositions(std::string _positions);
};