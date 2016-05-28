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
	_graphic.loadTexture(0, "bkg.png");

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
		imbs.Read(&pt, 4);
		if (pt == PacketType::PT_WELCOME)
		{
			int idSquare=0, numOtherPlayers=0, positionX=0, positionY=0;
			imbs.Read(&idSquare, 2);
			_network.SetIdSquare(idSquare);
			imbs.Read(&numOtherPlayers,2);
			for (size_t i = 0; i < numOtherPlayers; i++)
			{
				imbs.Read(&idSquare, 2);
				imbs.Read(&positionX, 10);
				imbs.Read(&positionY, 10);
				aSquares[idSquare].SetPosition(positionX, positionY);
			}
			imbs.Read(&idSquare, 2);
			imbs.Read(&positionX, 10);
			imbs.Read(&positionY, 10);
			aSquares[idSquare].SetPosition(positionX, positionY);
			_network.SetNetworkState(NetworkState::WELCOMED);
		}
		else if (pt == PacketType::PT_INIT)
		{
			int idSquare = 0, positionX = 0, positionY = 0;
			imbs.Read(&idSquare, 2);
			imbs.Read(&positionX, 10);
			imbs.Read(&positionY, 10);
			aSquares[idSquare].SetPosition(positionX, positionY);
		}
		else if (pt == PacketType::PT_FULL)
		{
			std::cout << "Servidor lleno" << std::endl;
		}
		else if (pt == PacketType::PT_SHOOT)
		{
			int posSquareX = 0, posSquareY = 0, vecShotX = 0, vecShotY = 0;
			imbs.Read(&posSquareX, 10);
			imbs.Read(&posSquareY, 10);
			imbs.Read(&vecShotX, 10);
			imbs.Read(&vecShotY, 10);
			shot newShot;
			newShot.px = posSquareX;
			newShot.py = posSquareY;
			newShot.mx = vecShotX;
			newShot.my = vecShotY;
			shotsList.push_back(newShot);
		}
		else if (pt == PacketType::PT_ROCK) {
			int size = 0, positionX = 0, positionY = 0, _desX = 0, _desY = 0;
			imbs.Read(&size, 4);
			for (int i = 0; i < size; i++) {
				rock newRock;
				imbs.Read(&positionX, 10);
				imbs.Read(&positionY, 10);
				imbs.Read(&_desX, 10);
				imbs.Read(&_desY, 10);
				newRock.Rock.SetPosition(positionX, positionY);
				newRock.desX = _desX;
				newRock.desY = _desY;
				aRocks.push_back(newRock);

			}

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
					//Si este movimiento lo realicé yo
					bool okPosition = _inputStateList.Acknowledge(idMove, confirmedPositionX, confirmedPositionY);

					if (!okPosition)
					{
						//Corrijo si la predicción esta mal.
						aSquares[idSquare].SetPosition(confirmedPositionX, confirmedPositionY);
					}
					std::cout << "Confirman: " << confirmedPositionX << " : " << confirmedPositionY << std::endl;
				}
				else
				{
					//Si este movimiento lo realizó otro cuadrado
					if (aStepsX.size() > 0 || aStepsY.size() > 0)
					{
						if (aStepsX.size() > NUM_STEPS_ENTITY)
						{
							//Si hay demasiados pasos a simular
							aStepsX = LittleSquareClient::CompressPath(NUM_STEPS_ENTITY, aStepsX);
						}
						if (aStepsY.size() > NUM_STEPS_ENTITY)
						{
							//Si hay demasiados pasos a simular
							aStepsY = LittleSquareClient::CompressPath(NUM_STEPS_ENTITY, aStepsY);
						}
						_aPlayersMoves.AddMoves(idSquare, aStepsX, aStepsY);
					}
				}
			}
		}
		else if (pt == PacketType::PT_RESETPLAYER)
		{
			int idSquare=0;
			imbs.Read(&idSquare, 2);
			aSquares[idSquare].SetPosition(330, 280);
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
		aSquares[playerMove.GetIdSquare()].SetDelta(playerMove.GetDeltaX(), playerMove.GetDeltaY());
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
		if (aSquares[_network.GetIdSquare()].GetPositionX() + 1 <= 660)
		{
			_inputState.AddRight();
			aSquares[_network.GetIdSquare()].AddRight();
		}
	}
	if (_graphic.isKeyDown(SDLK_LEFT)) {
		if (aSquares[_network.GetIdSquare()].GetPositionX() - 1 >= 0)
		{
			_inputState.AddLeft();
			aSquares[_network.GetIdSquare()].AddLeft();
		}
	}
	if (_graphic.isKeyDown(SDLK_UP)) {
		if (aSquares[_network.GetIdSquare()].GetPositionY() - 1 >= 0)
		{
			_inputState.AddUp();
			aSquares[_network.GetIdSquare()].AddUp();
		}
	}
	if (_graphic.isKeyDown(SDLK_DOWN)) {
		if (aSquares[_network.GetIdSquare()].GetPositionY() + 1 <= 560)
		{
			_inputState.AddDown();
			aSquares[_network.GetIdSquare()].AddDown();
		}
	}
	if (_graphic.isKeyPressed(SDLK_SPACE)) {
		shot newShot;
		newShot.px = aSquares[_network.GetIdSquare()].GetPositionX() + SIZE_SQUARE / 2;
		newShot.py = aSquares[_network.GetIdSquare()].GetPositionY() + SIZE_SQUARE / 2;
		int vec_x = _graphic.getMouseCoords().x - newShot.px;
		int vec_y = _graphic.getMouseCoords().y - newShot.py;
		float length = sqrt((vec_x * vec_x) + (vec_y * vec_y));
		float norm_x = vec_x / length;
		float norm_y = vec_y / length;
		newShot.mx = newShot.px + norm_x * 200;
		newShot.my = newShot.py + norm_y * 200;
		shotsList.push_back(newShot);
		_network.SendShot(newShot.px, newShot.py, newShot.mx, newShot.my, _inputState, _inputStateList);
	}
	if (_graphic.isKeyPressed(SDLK_ESCAPE)) {
		_gameState = GameState::EXIT;
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_DISCONNECT, 4);
		ombs.Write(_network.GetIdSquare());
		_network.Send(ombs.GetBufferPtr(), ombs.GetBitLength());
	}

}

/*
* Execute the game physics
*/
void Game::doPhysics() {
	
	//Shoot Decay
	for (int i = 0; i < shotsList.size(); i++) {
		shotsList[i].decay--;
		if (shotsList[i].decay < 0) { shotsList.erase(shotsList.begin() + i); };
	}

	//Rock Movement
	clock_t time = clock();
	if (time > 1000) {
		for (auto & element : aRocks) {
			element.Rock.SetPosition(element.Rock.GetPositionX() + element.desX * 20,
			element.Rock.GetPositionY() + element.desY * 20);
		}

		time = 0;
	}


	
	
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

	_graphic.drawTexture(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		int positionX = aSquares[i].GetPositionX();
		int positionY = aSquares[i].GetPositionY();
		_graphic.drawFilledRectangle(i, positionX, positionY, SIZE_SQUARE, SIZE_SQUARE);
	}
	
	for (auto & element : aRocks) {
		_graphic.drawFilledRectangle(BLACK, element.Rock.GetPositionX(), element.Rock.GetPositionY(), SIZE_SQUARE - 10, SIZE_SQUARE - 10);
	}
	
	for (auto & element : shotsList) {
		_graphic.drawLine(WHITE, element.px, element.py, element.mx, element.my);
	}

}

/*
* Draw the sprite on the screen
* @param sprite is the sprite to be displayed
*/
void Game::drawSprite(Sprite & sprite) {
	
}