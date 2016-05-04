#pragma once
#include "GenericSocket.h"

class UDPSocket:public GenericSocket
{
public:
	UDPSocket();
	int SendTo(const void* data, int lenData, SocketAddress& to);
	int ReceiveFrom(void* data, int lenData, SocketAddress& from);
	~UDPSocket();
};

