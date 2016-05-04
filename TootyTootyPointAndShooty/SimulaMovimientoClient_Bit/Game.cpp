#include "Game.h"
#include <InputMemoryStream.h>
#include <InputMemoryBitStream.h>
#include <OutputMemoryStream.h>
#include <OutputMemoryBitStream.h>

/**
* Constructor
* Tip: You can use an initialization list to set its attributes
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, std::string serverAddress, std::string clientAddress, std::string nick) :
	_windowTitle(windowTitle),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_gameState(GameState::INIT),
	_network(serverAddress, clientAddress, nick)
	
{
}

/**
* Destructor
*/
Game::~Game()
{
}

/*
* Game execution
*/
void Game::run() {
		//Prepare the game components
	init();
		//Start the game if everything is ready
	gameLoop();
}

/*
* Initialize all the components
*/
void Game::init() {
	srand((unsigned int)time(NULL));
		//Create a window
	_graphic.createWindow(_windowTitle, _screenWidth, _screenHeight, false);	
	_graphic.setWindowBackgroundColor(0, 0, 0, 255);
		//Set the font style
	_graphic.setFontStyle(TTF_STYLE_NORMAL);
		//Initialize the game elements
}


/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	_gameState = GameState::PLAY;
	
	while (_gameState != GameState::EXIT) {		
		//clock_t startTime = clock();
		if (_network.GetNetworkState() == NetworkState::SAYINGHELLO)
		{
			_network.SayHello();
		}

		int positionSquare = aSquares[_network.GetIdSquare()].GetPosition();
		_network.SendMove(positionSquare, _inputState, _inputStateList);
		
		Receiving();
		SimulateOtherPlayers();
		//Update the game physics
		doPhysics();
		//Detect keyboard and/or mouse events
		_graphic.detectInputEvents();
		//Execute the player commands 
		executePlayerCommands();
		//Render game
		renderGame();	
		//clock_t endTime = clock();
		//std::cout << "ITERACION: " << (endTime-startTime) << std::endl;
	}
}

void Game::Receiving()
{
	char buffer[MAX_BUFFER];
	int numBytes = _network.Receive(buffer);
	InputMemoryBitStream imbs(buffer, numBytes * 8);

	if (numBytes > 0)
	{
		//std::cout << "Recibo: " << strReceived << std::endl;
		PacketType pt = PacketType::PT_EMPTY;
		imbs.Read(&pt, 3);
		if (pt == PacketType::PT_WELCOME)
		{
			int idSquare=0, numOtherPlayers=0, position=0;
			imbs.Read(&idSquare, 1);
			_network.SetIdSquare(idSquare);
			imbs.Read(&numOtherPlayers,1);
			for (size_t i = 0; i < numOtherPlayers; i++)
			{
				imbs.Read(&idSquare, 1);
				imbs.Read(&position, 10);
				aSquares[idSquare].SetPosition(position);
			}
			_network.SetNetworkState(NetworkState::WELCOMED);
		}
		else if (pt == PacketType::PT_FULL)
		{
			std::cout << "Servidor lleno" << std::endl;
		}
		else if (pt == PacketType::PT_POSITION)
		{
			int numPositionsSend=0;
			imbs.Read(&numPositionsSend, 4);
			for (int i = 0; i < numPositionsSend; i++)
			{
				int idMove=0, idSquare=0, confirmedPosition=0;
				std::vector<int> aSteps;
				imbs.Read(&idMove);
				imbs.Read(&idSquare, 1);
				imbs.Read(aSteps);
				imbs.Read(&confirmedPosition, 10);
				int deltaMovement = LittleSquare::CalculateDeltaMovement(aSteps);

				if (idSquare == _network.GetIdSquare())
				{
					//Si este movimiento lo realicé yo
					bool okPosition = _inputStateList.Acknowledge(idMove, confirmedPosition);

					if (!okPosition)
					{
						//Corrijo si la predicción esta mal.
						aSquares[idSquare].SetPosition(confirmedPosition);
					}
					std::cout << "Confirman: " << confirmedPosition << std::endl;
				}
				else
				{
					//Si este movimiento lo realizó otro cuadrado
					if (aSteps.size() > 0)
					{
						if (aSteps.size() > NUM_STEPS_ENTITY)
						{
							//Si hay demasiados pasos a simular
							aSteps = LittleSquareClient::CompressPath(NUM_STEPS_ENTITY, aSteps);
						}
						_aPlayersMoves.AddMoves(idSquare, aSteps);
					}
				}
			}
		}
		else if (pt == PacketType::PT_RESETPLAYER)
		{
			int idSquare=0;
			imbs.Read(&idSquare, 1);
			aSquares[idSquare].SetPosition(MIN_SQUARE);
		}
		else if (pt == PacketType::PT_DISCONNECT)
		{
			_gameState = GameState::EXIT;
		}
	}
}

void Game::SimulateOtherPlayers()
{
	PlayerMove playerMove;
	bool okMove = _aPlayersMoves.PopMove(playerMove);
	if (okMove)
	{
		aSquares[playerMove.GetIdSquare()].SetDelta(playerMove.GetDelta());
	}
}

/**
* Executes the actions sent by the user by means of the keyboard and mouse
* Reserved keys:
- up | left | right | down moves the player object
- m opens the menu
*/
void Game::executePlayerCommands() {

	//Controlo que no se envíen movimientos inválidos.
	//Aunque si se enviaran el servidor lo controlaría y me devolvería al sitio.
	if (_graphic.isKeyDown(SDLK_RIGHT)){
		if (aSquares[_network.GetIdSquare()].GetPosition() + 1 <= MAX_SQUARE)
		{
			_inputState.AddRight();
			aSquares[_network.GetIdSquare()].AddRight();
		}
	}
	if (_graphic.isKeyDown(SDLK_LEFT)) {
		if (aSquares[_network.GetIdSquare()].GetPosition() - 1 >= MIN_SQUARE)
		{
			_inputState.AddLeft();
			aSquares[_network.GetIdSquare()].AddLeft();
		}
	}
	if (_graphic.isKeyPressed(SDLK_ESCAPE)) {
		_gameState = GameState::EXIT;
		OutputMemoryStream oms;
		oms.Write(PacketType::PT_DISCONNECT);
		oms.Write(_network.GetIdSquare());
		_network.Send(oms.GetBufferPtr(), oms.GetLength());
	}
}

/*
* Execute the game physics
*/
void Game::doPhysics() {
	

	
}

/**
* Render the game on the screen
*/
void Game::renderGame() {
		//Clear the screen
	_graphic.clearWindow();

		//Draw the screen's content based on the game state
	if (_gameState == GameState::MENU) {
		drawMenu();
	}
	else {
		drawGame();
	}
		//Refresh screen
	_graphic.refreshWindow();
}

/*
* Draw the game menu
*/
void Game::drawMenu() {

}

/*
* Draw the game, winner or loser screen
*/
void Game::drawGame() {

	_graphic.drawFilledRectangle(BLUE, 0, 0, 20, 300);
	_graphic.drawFilledRectangle(BLUE, 680, 0, 20, 300);
	
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		int position = aSquares[i].GetPosition();
		_graphic.drawFilledRectangle(i, position, 40 + 180*i, SIZE_SQUARE, SIZE_SQUARE);
	}
}

/*
* Draw the sprite on the screen
* @param sprite is the sprite to be displayed
*/
void Game::drawSprite(Sprite & sprite) {
	
}