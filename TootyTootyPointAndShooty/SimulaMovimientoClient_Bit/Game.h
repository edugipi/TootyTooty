#pragma once

//Third-party libraries
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <iostream>
#include <time.h>
#include "GameClientConstants.h"
#include "SDLInterface.h"
#include "InputManager.h"
#include "Sprite.h"
#include "NetworkClient.h"
#include "InputState.h"
#include "LittleSquareClient.h"
#include "PlayerMoveList.h"

struct shot{
	int px, py, mx, my;
	int decay = 15;
};

struct rock {
	LittleSquareClient Rock;
	int desX, desY;
};


/*
* The Game class manages the game execution
*/
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight, std::string serverAddress, std::string clientAddress, std::string nick);	//Constructor
		~Game();															//Destructor
		void run();															//Game execution	
		
	private:
			//Attributes	
		std::string _windowTitle;		//SDLInterface Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels				
		GameState _gameState;			//It describes the game state				
		SDLInterface _graphic;			//Manage the SDL graphic library		
		NetworkClient _network;
		InputState _inputState;
		InputStateList _inputStateList;
		PlayerMoveList _aPlayersMoves;
		LittleSquareClient aSquares[MAX_PLAYERS];
		std::vector <rock> aRocks;
		float norm_x;
		float norm_y;

		std::vector<shot> shotsList;
		
			//Internal methods for the game execution
		void init();
		void Receiving();
		void SimulateOtherPlayers();
		void gameLoop();		
		void executePlayerCommands();
		void doPhysics();
		void renderGame();
		void drawMenu();
		void drawGame();
		void drawSprite(Sprite & e);		
};

