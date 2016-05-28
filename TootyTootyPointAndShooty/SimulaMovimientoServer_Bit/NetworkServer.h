#pragma once
#include <UDPSocket.h>
#include <vector>
#include "ClientProxy.h"
#include "GameServerConstants.h"
#include "PlayerCommandList.h"

struct Roca{
	LittleSquare Rock;
	bool Active = true;
};

class NetworkServer
{
private:
	UDPSocket udpSocket;
	ClientProxy aPlayers[MAX_PLAYERS];
	Roca aRocks[8];
	bool aPlayersConnected[MAX_PLAYERS];
	PlayerCommandList aPlayersCommands;
	clock_t timeOfLastForward, timeOfLastSpawn;
	bool Dispatch_Message(char* _message, int _sizeMessage, SocketAddress _saClient);
	
	int ExistClientProxy(ClientProxy _clientProxy);
	int GetNumPlayers();
	int GetPositionFreePlayer();
public:
	NetworkServer(std::string _strServerAddress);
	bool Receive();
	void Dispatch_Forwards();
	void SendToAll(char* _message, int _size);
	~NetworkServer();
};