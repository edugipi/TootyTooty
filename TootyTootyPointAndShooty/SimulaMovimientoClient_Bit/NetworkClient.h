#pragma once
#include <UDPSocket.h>
#include "GameClientConstants.h"
#include <time.h>
#include "InputState.h"
#include "InputStateList.h"

class NetworkClient
{
private:
	UDPSocket udpSocket;
	SocketAddress saServer;
	std::string nick;
	int idSquare;
	NetworkState networkState;
	clock_t timeOfLastHello;
	clock_t timeOfLastMove;
public:
	NetworkClient(std::string _addressServer, std::string _addressClient, std::string _nick);
	
	NetworkState GetNetworkState();
	void SetNetworkState(NetworkState _networkState);
	int GetIdSquare();
	void SetIdSquare(int _idSquare);

	void SayHello();
	void SendMove(int _positionSquare, InputState& _inputState, InputStateList& _inputStateList);
	void Send(char* _message, int _size);
	int Receive(std::string& _message);
	int Receive(char* _message);
	
	~NetworkClient();
};

