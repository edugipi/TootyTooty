#pragma once

#include <iostream>
#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>
#include "GameClientConstants.h"
#include "InputManager.h"


//This class manages the window input/output using the SDL library
class SDLInterface 	{
public:
	SDLInterface();
	~SDLInterface();

		//Window methods
	int getWindowWidth();
	int getWindowHeight();
	int getWindowWidthFullScreen();
	int getWindowHeightFullScreen();
	void createWindow(std::string windowName, int screenWidth, int screenHeight, bool fullScreen);
	void setWindowBackgroundColor(int r, int g, int b, int a);
	void clearWindow();
	void refreshWindow();

		//Event methods
	int getCurrentTicks();
	void delayEvent(int timeMsecmsecs);
	static void pushExitGameEvent();
	void detectInputEvents();
	bool isKeyPressed(unsigned int keyID);
	bool isKeyDown(unsigned int keyID);
	glm::ivec2 getMouseCoords();

		//Texture methods
	void loadTexture(int textureId, std::string fileName);
	void getTextureDimension(int textureId, int & spriteWidth, int & spriteHeight);
	void drawTexture(int textureId, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int targetX, int targetY, int targetWidth, int targetHeight);
	void drawFlippedTexture(int textureId, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int targetX, int targetY, int targetWidth, int targetHeight, SDL_RendererFlip flip, float angle);	

		//Basic geometry methods
	void drawPoint(int color, int x, int y);
	void drawPoint(int r, int g, int b, int a, int x, int y);
	void drawLine(int color, int x1, int y1, int x2, int y2);
	void drawLine(int r, int g, int b, int a, int x1, int y1, int x2, int y2);
	void drawRectangle(int color, int x, int y, int width, int height);
	void drawRectangle(int r, int g, int b, int a, int x, int y, int width, int height);
	void drawFilledRectangle(int color, int x, int y, int width, int height);
	void drawFilledRectangle(int r, int g, int b, int a, int x, int y, int width, int height);

		//Text methods
	void setFontStyle(int style);
	void drawText(std::string text, int backgroundColor, int foregroundColor, int x, int y);
	void drawText(std::string text, int foregroundColor, int x, int y);

private:
	InputManager _inputManager;						//Manage the input devices
	TTF_Font* g_font;
	SDL_Texture* textures[GAME_TEXTURES];
	SDL_Color windowBackGroundColor;
	SDL_Color colorPalette[GAME_BASIC_COLORS];
	int _windowHeight, _windowWidth;
	int _windowHeightFullScreen, _windowWidthFullScreen;
	SDL_Window* _window = 0;
	SDL_Renderer* _renderer = 0;
};
