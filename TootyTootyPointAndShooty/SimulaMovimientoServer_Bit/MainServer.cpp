#include <string>
#include <SocketTools.h>
#include "NetworkServer.h"

void ServerRun(std::string _serverAddress)
{
	NetworkServer network(_serverAddress);
	do
	{
		network.Receive();
		network.Dispatch_Forwards();
		
	} while (true);
}

int main(int args, char* argv[])
{
	std::string myAddress = argv[1];
	SocketTools::CargarLibreria();
	ServerRun(myAddress);
	SocketTools::DescargarLibreria();
	return 0;
}