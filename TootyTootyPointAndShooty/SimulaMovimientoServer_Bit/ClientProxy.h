#pragma once
#include <string>
#include <time.h>
#include <SocketAddress.h>
#include <LittleSquare.h>

class ClientProxy
{
private:
	std::string nick;
	LittleSquare littleSquare;
	SocketAddress saClient;
	bool lastMoveWithError;
public:
	ClientProxy();
	ClientProxy(SocketAddress _saClient);
	ClientProxy(std::string _nick, SocketAddress _saClient);
	ClientProxy(const ClientProxy& _clientProxy);
	bool operator==(const ClientProxy& _clientProxy);
	int GetPositionSquare();
	int ChangeMove(int _move);
	bool GetErrorLastMove();
	void SetNick(std::string _nick);
	SocketAddress& GetSocketAddress();
	~ClientProxy();
};