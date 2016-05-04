#include "Game.h"
#include <SocketTools.h>

void PlayerRun(std::string serverAddress, std::string clientAddress, std::string nick)
{
	Game game("Movement", SCREEN_WIDTH, SCREEN_HEIGHT, serverAddress, clientAddress, nick);

	try {
		game.run();
	}
	catch (std::exception e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}



int main(int argc, char ** argv) {
	SocketTools::CargarLibreria();
	std::string serverAddress = argv[1];
	std::string clientAddress = argv[2];
	std::string nick = argv[3];

	PlayerRun(serverAddress, clientAddress, nick);

	SocketTools::DescargarLibreria();
	return 0;
}