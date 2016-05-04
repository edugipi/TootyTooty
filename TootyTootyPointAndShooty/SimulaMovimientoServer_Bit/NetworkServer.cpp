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
	imbs.Read(&pt, 3);

	ClientProxy cp(_saClient);
	int index = ExistClientProxy(cp);
	int freePosition = GetPositionFreePlayer();
	
	if (pt == PacketType::PT_HELLO)
	{
		std::cout << "Llega el HELLO" << std::endl;
		if (freePosition == -1)
		{
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_FULL, 3);
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
				ombs.Write(PacketType::PT_WELCOME, 3);
				//Corresponde al idSquare
				ombs.Write(freePosition, 1);
				int numPlayers = GetNumPlayers()-1;
				//Cuánta información recibiré de los demás usuarios
				//Preparado para más de 2
				ombs.Write(numPlayers, 1);
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (aPlayersConnected[i] && i != freePosition)
					{
						//Por cada uno de los demás jugadores paso 
						//su idSquare y su posición.
						ombs.Write(i, 1);
						ombs.Write(aPlayers[i].GetPositionSquare(), 10);
					}
				}
				udpSocket.SendTo(ombs.GetBufferPtr(), ombs.GetByteLength(), _saClient);
			}
		}
	}
	else if (pt == PacketType::PT_TRYPOSITION)
	{
		if (index != -1)
		{
			int idMove = 0;
			imbs.Read(&idMove);
			std::vector<int> aPositions;
			imbs.Read(aPositions);
			int delta = LittleSquare::CalculateDeltaMovement(aPositions);
			
			aPlayersCommands.AddCommand(index, idMove, delta, aPositions);
			
		}
	}
	else if (pt == PacketType::PT_DISCONNECT)
	{
		if (index != -1)
		{
			aPlayersConnected[index] = false;
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_RESETPLAYER, 3);
			ombs.Write(index, 1);
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
		ombs.Write(PacketType::PT_POSITION, 3);
		ombs.Write(aPlayersCommands.Size(), 4);
		for (size_t i = 0; i < aPlayersCommands.Size(); i++)
		{
			PlayerCommand playerCommand;
			aPlayersCommands.PopCommand(playerCommand);
			
			int idPlayer = playerCommand.GetIdPlayer();
			int delta = playerCommand.GetDelta();
			std::cout << "Intenta " << std::to_string(delta) << std::endl;
			int newPosition = aPlayers[idPlayer].ChangeMove(delta);
			std::cout << "Se concede " << std::to_string(newPosition) << std::endl;

			ombs.Write(playerCommand.GetIdMove());
			ombs.Write(idPlayer, 1);
			if (!aPlayers[idPlayer].GetErrorLastMove())
			{
				std::vector<int> aDetailedPath = playerCommand.GetDetailedPath();
				ombs.Write(aDetailedPath);
			}
			else
			{
				ombs.Write(0);
			}
			ombs.Write(newPosition, 10);
		}
		SendToAll(ombs.GetBufferPtr(), ombs.GetByteLength());
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
