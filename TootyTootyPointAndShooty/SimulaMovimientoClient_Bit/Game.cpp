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

		std::pair <int, int> positionSquare = std::make_pair(aSquares[_network.GetIdSquare()].GetPositionX(), aSquares[_network.GetIdSquare()].GetPositionY());
		_network.SendMove(positionSquare.first, positionSquare.second, _inputState, _inputStateList);
		
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
			int idSquare=0, numOtherPlayers=0, positionX=0, positionY=0;
			imbs.Read(&idSquare, 1);
			_network.SetIdSquare(idSquare);
			imbs.Read(&numOtherPlayers,1);
			for (size_t i = 0; i < numOtherPlayers; i++)
			{
				imbs.Read(&idSquare, 1);
				imbs.Read(&positionX, 10);
				imbs.Read(&positionY, 10);
				aSquares[idSquare].SetPosition(positionX, positionY);
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
				int idMove=0, idSquare=0, confirmedPositionX=0, confirmedPositionY=0;
				std::vector<int> aStepsX;
				std::vector<int> aStepsY;
				imbs.Read(&idMove);
				imbs.Read(&idSquare, 2);
				imbs.Read(aStepsX);
				imbs.Read(aStepsY);
				imbs.Read(&confirmedPositionX, 10);
				imbs.Read(&confirmedPositionY, 10);
				int deltaMovementX = LittleSquare::CalculateDeltaMovement(aStepsX);
				int deltaMovementY = LittleSquare::CalculateDeltaMovement(aStepsY);

				if (idSquare == _network.GetIdSquare())
				{
					//Si este movimiento lo realic� yo
					bool okPosition = _inputStateList.Acknowledge(idMove, confirmedPositionX, confirmedPositionY);

					if (!okPosition)
					{
						//Corrijo si la predicci�n esta mal.
						aSquares[idSquare].SetPosition(confirmedPositionX, confirmedPositionY);
					}
					std::cout << "Confirman: " << confirmedPositionX << " : " << confirmedPositionY << std::endl;
				}
				else
				{
					//Si este movimiento lo realiz� otro cuadrado
					if (aStepsX.size() > 0)
					{
						if (aStepsX.size() > NUM_STEPS_ENTITY)
						{
							//Si hay demasiados pasos a simular
							aStepsX = LittleSquareClient::CompressPath(NUM_STEPS_ENTITY, aStepsX);
						}
						_aPlayersMoves.AddMovesX(idSquare, aStepsX);
					}
					if (aStepsY.size() > 0)
					{
						if (aStepsY.size() > NUM_STEPS_ENTITY)
						{
							//Si hay demasiados pasos a simular
							aStepsY = LittleSquareClient::CompressPath(NUM_STEPS_ENTITY, aStepsY);
						}
						_aPlayersMoves.AddMovesY(idSquare, aStepsY);
					}
				}
			}
		}
		else if (pt == PacketType::PT_RESETPLAYER)
		{
			int idSquare=0;
			imbs.Read(&idSquare, 2);
			aSquares[idSquare].SetPosition(MIN_SQUARE, MIN_SQUARE);
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
		aSquares[playerMove.GetIdSquare()].SetDelta(playerMove.GetDeltaX(),playerMove.GetDeltaY());
	}
}

/**
* Executes the actions sent by the user by means of the keyboard and mouse
* Reserved keys:
- up | left | right | down moves the player object
- m opens the menu
*/
void Game::executePlayerCommands() {

	//Controlo que no se env�en movimientos inv�lidos.
	//Aunque si se enviaran el servidor lo controlar�a y me devolver�a al sitio.
	if (_graphic.isKeyDown(SDLK_RIGHT)){
		if (aSquares[_network.GetIdSquare()].GetPositionX() + 1 <= MAX_SQUARE)
		{
			_inputState.AddRight();
			aSquares[_network.GetIdSquare()].AddRight();
		}
	}
	if (_graphic.isKeyDown(SDLK_LEFT)) {
		if (aSquares[_network.GetIdSquare()].GetPositionX() - 1 >= MIN_SQUARE)
		{
			_inputState.AddLeft();
			aSquares[_network.GetIdSquare()].AddLeft();
		}
	}
	if (_graphic.isKeyDown(SDLK_UP)) {
		if (aSquares[_network.GetIdSquare()].GetPositionX() - 1 <= MAX_SQUARE)
		{
			_inputState.AddUp();
			aSquares[_network.GetIdSquare()].AddUp();
		}
	}
	if (_graphic.isKeyDown(SDLK_DOWN)) {
		if (aSquares[_network.GetIdSquare()].GetPositionX() + 1 >= MIN_SQUARE)
		{
			_inputState.AddDown();
			aSquares[_network.GetIdSquare()].AddDown();
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
		int positionX = aSquares[i].GetPositionX();
		int positionY = aSquares[i].GetPositionY();
		_graphic.drawFilledRectangle(i, positionX, positionY, SIZE_SQUARE, SIZE_SQUARE);
	}
}

/*
* Draw the sprite on the screen
* @param sprite is the sprite to be displayed
*/
void Game::drawSprite(Sprite & sprite) {
	
}