#include "ErrorManagement.h"
#include <SDL/SDL.h>
#include <iostream>
#include "SDLInterface.h"

/*
* Prints out an error message and exits the game
*/
void ErrorManagement::errorRunTime(std::string errorString) {
	cout << errorString << endl;
	cout << "Program will exit. Enter any key to continue";
	system("pause");
	SDLInterface::pushExitGameEvent();	
}
