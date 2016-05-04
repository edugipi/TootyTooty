#pragma once
#include "GenericSocket.h"
#include <memory>
class TCPSocket :
	public GenericSocket
{
public:
	TCPSocket();
	TCPSocket(SOCKET _sock);

	int Listen(int backLog);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& from);
	int Connect(SocketAddress& address);
	int Send(const void* data);
	int Receive(void* data, int lenData);
	~TCPSocket();
};

