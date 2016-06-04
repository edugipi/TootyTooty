#include "NetworkServer.h"
#include "GameServerConstants.h"
#include <LittleSquare.h>
#include <OutputMemoryStream.h>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#include <InputMemoryStream.h>


#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 600

NetworkServer::NetworkServer(std::string _strServerAddress): timeOfLastForward(0), timeOfLastSpawn(0)
{
	SocketAddress saServer;
	saServer.SetAddress(_strServerAddress);
	udpSocket.Bind(saServer);
	udpSocket.NonBlocking(true);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		aPlayersConnected[i] = false;
	}
	aRocks[0].Rock.SetPosition(SCREEN_WIDTH/3, 0);
	aRocks[1].Rock.SetPosition(SCREEN_WIDTH * 2 / 3, 0);
	aRocks[2].Rock.SetPosition(SCREEN_WIDTH + 10, SCREEN_HEIGHT / 3);
	aRocks[3].Rock.SetPosition(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2 / 3);
	aRocks[4].Rock.SetPosition(SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT + 10);
	aRocks[5].Rock.SetPosition(SCREEN_WIDTH / 3, SCREEN_HEIGHT + 10);
	aRocks[6].Rock.SetPosition(0, SCREEN_HEIGHT * 2 / 3);
	aRocks[7].Rock.SetPosition(0, SCREEN_HEIGHT / 3);
}

bool NetworkServer::Receive()
{
	bool exit = false;
	char buffer[MAX_BUFFER];
	SocketAddress from;

	int numBytes = udpSocket.ReceiveFrom(buffer, MAX_BUFFER, from);
	if (numBytes > 0)
	{
		if (numBytes < MAX_BUFFER)
		{
			buffer[numBytes] = '\0';
		}
		exit = Dispatch_Message(buffer, numBytes, from);
	}
	return exit;
}

void NetworkServer::SendToAll(char* _message, int _size)
{
	for (size_t i = 0; i < MAX_PLAYERS; i++)
	{
		if (aPlayersConnected[i])
		{
			SocketAddress saClient = aPlayers[i].GetSocketAddress();
			udpSocket.SendTo(_message, _size, saClient);
		}
	}
}

NetworkServer::~NetworkServer()
{

}

bool NetworkServer::Dispatch_Message(char* _message, int _sizeMessage, SocketAddress _saClient)
{
	//InputMemoryStream ims(_message, _sizeMessage);
	InputMemoryBitStream imbs(_message, _sizeMessage * 8);
	PacketType pt=PacketType::PT_EMPTY;
	imbs.Read(&pt, 4);

	ClientProxy cp(_saClient);
	int index = ExistClientProxy(cp);
	int freePosition = GetPositionFreePlayer();
	
	if (pt == PacketType::PT_HELLO)
	{
		std::cout << "Llega el HELLO" << std::endl;
		if (freePosition == -1)
		{
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_FULL, 4);
			udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), _saClient);
		}
		else
		{
			if (index == -1)
			{
				std::string nick;
				imbs.ReadString(&nick);
				std::cout << "Llega " << nick << std::endl;
				cp.SetNick(nick);
				aPlayers[freePosition] = cp;
				aPlayersConnected[freePosition] = true;
				
				OutputMemoryBitStream ombs;
				ombs.Write(PacketType::PT_WELCOME, 4);
				//Corresponde al idSquare
				ombs.Write(freePosition, 2);
				int numPlayers = GetNumPlayers()-1;
				//Cuánta información recibiré de los demás usuarios
				//Preparado para más de 2
				ombs.Write(numPlayers, 2);
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (aPlayersConnected[i] && i != freePosition)
					{
						//Por cada uno de los demás jugadores paso 
						//su idSquare y su posición.
						ombs.Write(i, 2);
						ombs.Write(aPlayers[i].GetPositionSquare().first, 10);
						ombs.Write(aPlayers[i].GetPositionSquare().second, 10);
					}
				}
				ombs.Write(freePosition, 2);
				switch (freePosition) {
				case 0:
					ombs.Write(290, 10);
					ombs.Write(240, 10);
					aPlayers[0].SetPositionSquare(290, 240);
					break;
				case 1:
					ombs.Write(370, 10);
					ombs.Write(240, 10);
					aPlayers[1].SetPositionSquare(370, 240);
					break;
				case 2:
					ombs.Write(290, 10);
					ombs.Write(320, 10);
					aPlayers[2].SetPositionSquare(290, 320);
					break;
				case 3:
					ombs.Write(370, 10);
					ombs.Write(320, 10);
					aPlayers[3].SetPositionSquare(370, 320);
					break;
				}
				udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), _saClient);
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (aPlayersConnected[i] && i != freePosition)
					{
						//Por cada uno de los demás jugadores paso un paquete con tu posición
						OutputMemoryBitStream ombs;
						ombs.Write(PacketType::PT_INIT, 4);
						ombs.Write(freePosition, 2);
						ombs.Write(aPlayers[freePosition].GetPositionSquare().first, 10);
						ombs.Write(aPlayers[freePosition].GetPositionSquare().second, 10);
						udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), aPlayers[i].GetSocketAddress());
					}
				}
				if (GetNumPlayers() == 2) {
					for (int i = 0; i < MAX_ROCKS; i++) {
						aRocks[i].Active = true;
					}
				}
			}
		}
	}
	else if (pt == PacketType::PT_TRYPOSITION)
	{
		if (index != -1)
		{
			int idMove = 0;
			imbs.Read(&idMove);
			std::vector<int> aPosX;
			std::vector<int> aPosY;
			imbs.Read(aPosX);
			imbs.Read(aPosY);
			int deltaX = LittleSquare::CalculateDeltaMovement(aPosX);
			int deltaY = LittleSquare::CalculateDeltaMovement(aPosY);
			
			aPlayersCommands.AddCommand(index, idMove, deltaX, deltaY, aPosX, aPosY);
			
		}
	}
	else if (pt == PacketType::PT_SHOOT)
	{
		if (index != -1)
		{
			int id = 0, posSquareX = 0, posSquareY = 0, vecShotX = 0, vecShotY = 0;
			imbs.Read(&posSquareX, 10);
			imbs.Read(&posSquareY, 10);
			imbs.Read(&vecShotX, 10);
			imbs.Read(&vecShotY, 10);
			//Check if rock shot
			SDL_Rect rect;
		
			for (int i = 0; i < MAX_ROCKS; i++) {
				rect.x = aRocks[i].Rock.GetPositionX();
				rect.y = aRocks[i].Rock.GetPositionY();
				rect.w = 40;
				rect.h = 40;
				if (SDL_IntersectRectAndLine(&rect,
					&posSquareX, &posSquareY, &vecShotX, &vecShotY)) {
					switch (i) {
					case 0:
					case 1:
						aRocks[i].Rock.SetPosition(50 + (rand() % (SCREEN_WIDTH - 100)), 0);
						break;
					case 2:
					case 3:
						aRocks[i].Rock.SetPosition(SCREEN_WIDTH, 50 + (rand() % (SCREEN_HEIGHT - 100)));
						break;
					case 4:
					case 5:
						aRocks[i].Rock.SetPosition(50 + (rand() % (SCREEN_WIDTH - 100)), SCREEN_HEIGHT);
						break;
					case 6:
					case 7:
						aRocks[i].Rock.SetPosition(0, 50 + (rand() % (SCREEN_HEIGHT - 100)));
						break;
					}
					aRocks[i].Active = true;

					std::cout << "Tocada" << std::endl;
				}
			}
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				if (i != index && aPlayersConnected[i])
				{
					//Por cada uno de los demás jugadores paso un paquete con tu posición
					OutputMemoryBitStream ombs;
					ombs.Write(PacketType::PT_SHOOT, 4);
					ombs.Write(posSquareX, 10);
					ombs.Write(posSquareY, 10);
					ombs.Write(vecShotX, 10);
					ombs.Write(vecShotY, 10);

					udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), aPlayers[i].GetSocketAddress());
				}
			}
		}
	}
	else if (pt == PacketType::PT_DISCONNECT)
	{
		if (index != -1)
		{
			aPlayersConnected[index] = false;
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_RESETPLAYER, 4);
			ombs.Write(index, 2);
			SendToAll(ombs.GetBufferPtr(), ombs.GetByteLength());
		}
		if (GetNumPlayers() == 0)
		{
			return true;
		}
	}
	return false;
}

void NetworkServer::Dispatch_Forwards()
{
	clock_t time = clock();
	if (time > timeOfLastForward + FREQUENCY_SENDING_WORLD)
	{
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_POSITION, 4);
		ombs.Write(aPlayersCommands.Size(), 4);
		for (size_t i = 0; i < aPlayersCommands.Size(); i++)
		{
			PlayerCommand playerCommand;
			aPlayersCommands.PopCommand(playerCommand);
			
			int idPlayer = playerCommand.GetIdPlayer();
			int deltaX = playerCommand.GetDeltaX();
			int deltaY = playerCommand.GetDeltaY();
			std::cout << "Intenta " << std::to_string(deltaX) << " : " << std::to_string(deltaY) << std::endl;
			std::pair <int, int> newPosition = aPlayers[idPlayer].ChangeMove(deltaX, deltaY);
			std::cout << "Se concede " << std::to_string(newPosition.first) << " : " << std::to_string(newPosition.second) << std::endl;

			ombs.Write(playerCommand.GetIdMove());
			ombs.Write(idPlayer, 2);
			if (!aPlayers[idPlayer].GetErrorLastMove()) {
				std::vector<int> aDetailedPathX = playerCommand.GetDetailedPathX();
				std::vector<int> aDetailedPathY = playerCommand.GetDetailedPathY();
				ombs.Write(aDetailedPathX);
				ombs.Write(aDetailedPathY);

			} else {
				ombs.Write(0, 10);
				ombs.Write(0, 10);

			} ombs.Write(newPosition.first, 10);
			ombs.Write(newPosition.second, 10);

		} SendToAll(ombs.GetBufferPtr(), ombs.GetByteLength());
		timeOfLastForward = time;

	}
	if (time > timeOfLastMovement + 50) {
		for (int i = 0; i < MAX_ROCKS; i++) {
			if (!aRocks[i].Active) {
				aRocks[i].Rock.SetPosition(aRocks[i].Rock.GetPositionX() + (aRocks[i].desX * 4),
					aRocks[i].Rock.GetPositionY() + (aRocks[i].desY * 4));
			}
			switch (i) {
			case 0:
			case 1:
				if (aRocks[i].Rock.GetPositionY() > SCREEN_HEIGHT + 20) {
					aRocks[i].Rock.SetPosition(50 + (rand() % (SCREEN_WIDTH - 100)), 0);
					aRocks[i].Active = true;
				}
				break;
			case 2:
			case 3:
				if (aRocks[i].Rock.GetPositionX() < - 20) {
					aRocks[i].Rock.SetPosition(SCREEN_WIDTH, 50 + (rand() % (SCREEN_HEIGHT- 100)));
					aRocks[i].Active = true;
				}
				break;
			case 4:
			case 5:
				if (aRocks[i].Rock.GetPositionY() < - 20) {
					aRocks[i].Rock.SetPosition(50 + (rand() % (SCREEN_WIDTH - 100)), SCREEN_HEIGHT);
					aRocks[i].Active = true;
				}
				break;
			case 6:
			case 7:
				if (aRocks[i].Rock.GetPositionX() > SCREEN_WIDTH + 20) {
					aRocks[i].Rock.SetPosition(0, 50 + (rand() % (SCREEN_HEIGHT - 100)));
					aRocks[i].Active = true;
				}
				break;
			}
		}
		timeOfLastMovement = time;
	}
	if (time > timeOfLastSpawn + SPAWN_TIME) {
		//if (GetNumPlayers() == 2) {
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_ROCK, 4);

			std::vector<Roca> activeRocks;
			for (int i = 0; i < MAX_ROCKS; i++) {
				if (aRocks[i].Active == true) {
					activeRocks.push_back(aRocks[i]);
					int vec_x = SCREEN_WIDTH / 2 - aRocks[i].Rock.GetPositionX();
					int vec_y = SCREEN_HEIGHT / 2 - aRocks[i].Rock.GetPositionY();
					float length = sqrt((vec_x * vec_x) + (vec_y * vec_y));
					aRocks[i].desX = vec_x / length;
					aRocks[i].desY = vec_y / length;
					aRocks[i].Active = false;
				}
			}
			ombs.Write(activeRocks.size(), 4);
			for (auto & element : activeRocks) {
				ombs.Write(element.Rock.GetPositionX(), 10);
				ombs.Write(element.Rock.GetPositionY(), 10);/*
				int vec_x = element.Rock.GetPositionX() + std::rand() % SCREEN_WIDTH + element.Rock.GetPositionX();
				int vec_y = element.Rock.GetPositionY() + std::rand() % SCREEN_WIDTH + element.Rock.GetPositionY();*/
				element.desX = SCREEN_WIDTH / 2;
				element.desY = SCREEN_HEIGHT / 2;
				ombs.Write(element.desX, 10);
				ombs.Write(element.desY, 10);
			}
			SendToAll(ombs.GetBufferPtr(), ombs.GetByteLength());

			timeOfLastSpawn = time;
		//}
	}
	
}

int NetworkServer::ExistClientProxy(ClientProxy _clientProxy)
{
	for (size_t i = 0; i < MAX_PLAYERS; i++)
	{
		if (aPlayersConnected[i] && aPlayers[i] == _clientProxy)
		{
			return i;
		}
	}
	return -1;
}

int NetworkServer::GetNumPlayers()
{
	int numPlayers = 0;
	for (size_t i = 0; i < MAX_PLAYERS; i++)
	{
		if (aPlayersConnected[i] == true)
		{
			numPlayers++;
		}
	}
	return numPlayers;
}

int NetworkServer::GetPositionFreePlayer()
{
	int numPlayers = GetNumPlayers();
	if (numPlayers < MAX_PLAYERS)
	{
		for (size_t i = 0; i < MAX_PLAYERS; i++)
		{
			if (!aPlayersConnected[i])
			{
				return i;
			}
		}
	}
	return -1;
}
