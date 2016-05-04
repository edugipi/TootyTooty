#include "UDPSocket.h"
#include "SocketTools.h"


UDPSocket::UDPSocket():GenericSocket(SOCK_DGRAM)
{
}

int UDPSocket::SendTo(const void * data, int lenData, SocketAddress & to)
{
	sockaddr sa;
	to.GetAddress(sa);
	int numBytesSend = sendto(sock, (const char*)data, lenData, 0, &sa, sizeof(sa));
	if (numBytesSend >= 0)
	{
		return numBytesSend;
	}
	else
	{
		SocketTools::MostrarError("Error en UDPSocket::SendTo");
		return -1;
	}
}

int UDPSocket::ReceiveFrom(void * data, int lenData, SocketAddress & from)
{
	sockaddr sa;
	int sizeFrom = sizeof(sockaddr);
	int numBytesReceived = recvfrom(sock, (char*)data, lenData, 0, &sa, &sizeFrom);
	if (numBytesReceived >= 0)
	{
		from.SetAddress(sa);
		return numBytesReceived;
	}
	else
	{
		int error = WSAGetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			return -WSAECONNRESET;
		}
		else
		{
			SocketTools::MostrarError("UDPSocket::ReceiveFrom");
			return -error;
		}
	}
}


UDPSocket::~UDPSocket()
{
}
