#include "TCPSocket.h"
#include "SocketTools.h"




TCPSocket::TCPSocket() :GenericSocket(SOCK_STREAM)
{
}

TCPSocket::TCPSocket(SOCKET _sock) : GenericSocket(SOCK_STREAM)
{
	sock = _sock;
}



int TCPSocket::Listen(int backLog)
{
	int err = listen(sock, backLog);
	if (err == -1)
	{
		SocketTools::MostrarError("Error en TCPSocket::Listen");
	}
	return err;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress & from)
{
	sockaddr addr;
	int addrLen = sizeof(sockaddr);
	SOCKET sockAux = accept(sock, &addr, &addrLen);
	from.SetAddress(addr);
	std::shared_ptr<TCPSocket> tcpSocket = std::shared_ptr<TCPSocket>(new TCPSocket(sockAux));
	return tcpSocket;
}

int TCPSocket::Connect(SocketAddress & address)
{
	sockaddr sa;
	address.GetAddress(sa);
	int err = connect(sock, &sa, sizeof(sockaddr));
	if (err == -1)
	{
		SocketTools::MostrarError("Error en TCPSocket::Connect");
	}
	return err;
}

int TCPSocket::Send(const void * data)
{
	int bytesSent = send(sock, (const char*)data, 1300, 0);
	if (bytesSent == -1)
	{
		SocketTools::MostrarError("Error en TCPSocket::Send");
	}
	return bytesSent;
}

int TCPSocket::Receive(void * data, int lenData)
{
	int bytesReceived = recv(sock, (char*)data, lenData, 0);
	if (bytesReceived == -1)
	{
		SocketTools::MostrarError("Error en TCPSocket::Receive");
	}
	else if (bytesReceived == 0 && lenData > 0)
	{
		std::cout << "FIN Packet en TCPSocket::Receive" << std::endl;
	}
	return bytesReceived;
}


TCPSocket::~TCPSocket()
{
	shutdown(sock, SD_BOTH);
}
