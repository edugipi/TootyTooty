#include "SocketTools.h"
#include <iostream>

int SocketTools::CargarLibreria()
{
	WSAData data;
	int err = WSAStartup(MAKEWORD(2,2), &data);
	if (err == -1)
	{
		MostrarError("Error en SocketTools::DescargarLibrería");
	}
	return err;
}

int SocketTools::DescargarLibreria()
{
	int err = WSACleanup();
	if (err == -1)
	{
		MostrarError("Error en SocketTools::DescargarLibrería");
	}
	return err;
}


void SocketTools::MostrarError(std::string mensaje)
{
	int err = WSAGetLastError();
	if (err != EAGAIN && err != WSAEWOULDBLOCK)
	{
		std::cout << mensaje << ". ErrNo: " << err << std::endl;
	}
}
