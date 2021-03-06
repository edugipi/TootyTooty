#include "NetworkServer.h"
#include "GameServerConstants.h"
#include <LittleSquare.h>
#include <OutputMemoryStream.h>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#include <InputMemoryStream.h>

NetworkServer::NetworkServer(std::string _strServerAddress): timeOfLastForward(0)
{
	SocketAddress saServer;
	saServer.SetAddress(_strServerAddress);
	udpSocket.Bind(saServer);
	udpSocket.NonBlocking(true);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		aPlayersConnected[i] = false;
	}
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
				//Cu�nta informaci�n recibir� de los dem�s usuarios
				//Preparado para m�s de 2
				ombs.Write(numPlayers, 2);
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (aPlayersConnected[i] && i != freePosition)
					{
						//Por cada uno de los dem�s jugadores paso 
						//su idSquare y su posici�n.
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
						//Por cada uno de los dem�s jugadores paso un paquete con tu posici�n
						OutputMemoryBitStream ombs;
						ombs.Write(PacketType::PT_INIT, 4);
						ombs.Write(freePosition, 2);
						ombs.Write(aPlayers[freePosition].GetPositionSquare().first, 10);
						ombs.Write(aPlayers[freePosition].GetPositionSquare().second, 10);
						udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), aPlayers[i].GetSocketAddress());
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
