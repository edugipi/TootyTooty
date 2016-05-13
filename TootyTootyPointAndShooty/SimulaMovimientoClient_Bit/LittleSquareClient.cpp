#include "LittleSquareClient.h"

bool LittleSquareClient::IsValidPosition() {
	if (pos_x >= MIN_SQUARE && pos_x <= MAX_SQUARE && pos_y >= MIN_SQUARE && pos_y <= MAX_SQUARE) {
		return true;
	} return false;

}

void LittleSquareClient::SetDelta(int _deltaX, int _deltaY)
{
	//Compruebo el delta que ha hecho el otro jugador, para no simular posiciones erróneas
	//Lo corrijo aquí para ver qué pasaría si el otro jugador tratara de hacer trampas.
	//Además se podría añadir que no se permitiera el movimiento más allá de los límites.
	//int checkPosition = position + _delta;
	//if (checkPosition >= MIN_SQUARE && checkPosition <= MAX_SQUARE)
	//{
		pos_x += _deltaX;
		pos_y += _deltaY;
	//}
}

std::pair <int, int> LittleSquareClient::CalculateEndPosition(int _curPosX, int _curPosY, PlayerMoveList & _aPlayersMovesX, PlayerMoveList & _aPlayersMovesY)
{
	for (size_t i = 0; i < _aPlayersMovesX.size(); i++) {
		PlayerMove playerMove = _aPlayersMovesX[i];
		_curPosX += playerMove.GetDeltaX();
	}
	for (size_t i = 0; i < _aPlayersMovesY.size(); i++) {
		PlayerMove playerMove = _aPlayersMovesY[i];
		_curPosY += playerMove.GetDeltaX();
	}
	return std::make_pair (_curPosX, _curPosY);
}

void LittleSquareClient::InterpolatePath(int _start, int _end, std::string& _strPositions)
{
	_strPositions = "";
	if (_start < _end)
	{
		for (int i = _start; i < _end; i++)
		{
			_strPositions += std::to_string(1);
			if (i + 1 < _end)
			{
				_strPositions += ":";
			}
		}
	}
	else if (_end < _start)
	{
		for (int i = _end; i < _start; i++)
		{
			_strPositions += std::to_string(-1);
			if (i + 1 < _start)
			{
				_strPositions += ":";
			}
		}
	}

}

void LittleSquareClient::InterpolatePath(int _start, int _end, std::vector<int>& _aPositions)
{
	_aPositions.clear();
	std::string str;
	InterpolatePath(_start, _end, str);
	_aPositions = LittleSquareClient::ParserListPositions(str);
	
}

std::vector<int> LittleSquareClient::CompressPath(int _numSteps, std::vector<int>& _aPositions)
{
	std::vector<int> aCompressPath;
	int numCells = _aPositions.size();
	int cellPerStep = ((numCells) / (_numSteps))+1;
	
	int currentIndex = 0;
	while (true)
	{

		int acum = 0;
		for (int i = currentIndex; i < (currentIndex+cellPerStep) && i<numCells; i++)
		{
			acum += _aPositions[i];
		}
		aCompressPath.push_back(acum);
		currentIndex = currentIndex + cellPerStep;
		if (currentIndex >= numCells)
		{
			break;
		}
	}
	return aCompressPath;
}

std::vector<int> LittleSquareClient::ParserListPositions(std::string _positions)
{
	std::vector<int> aPositions;
	if (_positions.size() == 0)
		return aPositions;
	int index2Ptos = _positions.find_first_of(':');
	if (index2Ptos == std::string::npos)
	{
		aPositions.push_back(atoi(_positions.c_str()));
	}
	while (index2Ptos != std::string::npos)
	{
		std::string strPosition = _positions.substr(0, index2Ptos);
		std::string strTheOthers = _positions.substr(index2Ptos + 1, _positions.size() - index2Ptos);
		aPositions.push_back(atoi(strPosition.c_str()));
		index2Ptos = strTheOthers.find_first_of(':');
		if (index2Ptos == std::string::npos)
		{
			aPositions.push_back(atoi(strTheOthers.c_str()));
		}
		_positions = strTheOthers;
	} 
	
	return aPositions;
}
