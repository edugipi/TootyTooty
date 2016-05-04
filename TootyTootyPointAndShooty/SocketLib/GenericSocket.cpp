#include "GenericSocket.h"
#include "SocketTools.h"


GenericSocket::GenericSocket(int type)
{
	sock = socket(AF_INET, type, 0);
	if (sock == INVALID_SOCKET)
	{
		SocketTools::MostrarError("Error en GenericSocket constructor");
	}
}

int GenericSocket::Bind(SocketAddress & address)
{
	sockaddr dir;
	address.GetAddress(dir);
	int err = bind(sock, &dir, sizeof(sockaddr));
	if (err == -1)
	{
		SocketTools::MostrarError("Error en GenericSocket::Bind");
	}
	return err;
}

int GenericSocket::NonBlocking(bool esNonBlocking)
{
	u_long pBlock;
	if (esNonBlocking)
	{
		pBlock = 1;
	}
	else
	{
		pBlock = 0;
	}
	int err = ioctlsocket(sock, FIONBIO, &pBlock);
	if (err == -1)
	{
		SocketTools::MostrarError("Error en GenericSocket::NonBlocking");
	}
	return err;
}

GenericSocket::~GenericSocket()
{
	int err = closesocket(sock);
	if (err == -1)
	{
		SocketTools::MostrarError("Error al GenericSocket::~GenericSocket");
	}
}
