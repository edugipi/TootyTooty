#pragma once
#include "SocketAddress.h"

class GenericSocket
{
protected:
	SOCKET sock;
public:
	GenericSocket(int type);
	int Bind(SocketAddress& sa);
	int NonBlocking(bool esNonBlocking);
	virtual ~GenericSocket();
};

