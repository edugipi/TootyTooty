#include "SDLInterface.h"
#include <SDL/SDL.h>
#include <SDL_image\SDL_image.h>
#include "ErrorManagement.h"


/**
* Initialize the game engine
*/
SDLInterface::SDLInterface() 	{	
		//Initialize all the SDL' subsystems, even they are not used for this example
		//See SDL initialization flags for more details: SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ErrorManagement::errorRunTime("[SDLInterface] SDL cannot be initialized");
	}

		//Initialize TTF fonts from SDL_ttf. Return 0 if it is successful
	if (TTF_Init() == 0){
		g_font = TTF_OpenFont("../sharedResources/fonts/arial.ttf", 20);
		if (g_font == 0){
			ErrorManagement::errorRunTime("[SDLInterface] The font file cannot be loaded");
		}
	}else {
		ErrorManagement::errorRunTime("[SDLInterface] SDL TTF cannot be initialized");
	}

		//Initialize the color palette
	for (int i = 0; i < GAME_BASIC_COLORS; i++) {
		colorPalette[i].r = 0;
		colorPalette[i].g = 0;
		colorPalette[i].b = 0;
		colorPalette[i].a = 255;
	}
	
	colorPalette[RED].r = 255;
	colorPalette[GREEN].g = 255;
	colorPalette[BLUE].b = 255;
	colorPalette[WHITE].r = 255;
	colorPalette[WHITE].g = 255;
	colorPalette[WHITE].b = 255;	
}

/**
* Destructor
*/
SDLInterface::~SDLInterface() 	{
		//Destroy Textures
	for (int i = 0; i < GAME_TEXTURES; i++) {
		SDL_DestroyTexture(textures[i]);
	}

		//Destroy the renderer and the window
	if (_renderer!=0)
		SDL_DestroyRenderer(_renderer);
	if (_window != 0) 
		SDL_DestroyWindow(_window);		

		//Clean up SDL
	SDL_Quit();
}

/*
* Get the width of the original window
* @return the int value of the width of the original window
*/
int SDLInterface::getWindowWidth() { 
		return _windowWidth; 
}

/*
* Get the height of the original window
* @return the int value of the height of the original window
*/
int SDLInterface::getWindowHeight() { 
	return _windowHeight; 
}

/*
* Get the width of the window in the full screen mode
* @return the int value of the width of the window in the full screen mode
*/
int SDLInterface::getWindowWidthFullScreen() { 
	return _windowWidthFullScreen; 
}

/*
* Get the height of the window in the full screen mode
* @return the int value of the height of the window in the full screen mode
*/
int SDLInterface::getWindowHeightFullScreen() {
	return _windowHeightFullScreen; 
}

/*
* Create a new window
* @param windowName is the window's title
* @param windowWidth is the window's width
* @param windowHeight is the window's height
* @param fullScreen specifies if the window is full screen or not
*/
void SDLInterface::createWindow(std::string windowName, int windowWidth, int windowHeight, bool fullScreen) {
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	
		//Update the window flags
	Uint32 flags = SDL_WINDOW_SHOWN;
	if (fullScreen) {
		flags |= SDL_WINDOW_MAXIMIZED | SDL_WINDOW_BORDERLESS;
	}	

		//Create a SDL window centered into the middle of the screen
	_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, flags);

		//Declare display mode structure to be filled in. It will allow us to get the current resolution
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	_windowWidthFullScreen = current.w;
	_windowHeightFullScreen = current.h;
			
		// if the window creation succeeded create our renderer.
	if (_window != 0) {
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
			// Set to black
		setWindowBackgroundColor(0, 0, 0, 255);		
	}
	else {
		ErrorManagement::errorRunTime("[SDL Interface] A problem occurred when the window was created");
	}	
}

/*
* Define the background color of the window using the RGBA components
* @param r is the red component
* @param g is the green component
* @param b is the blue component
* @param a is the alpha component
*/
void SDLInterface::setWindowBackgroundColor(int r, int g, int b, int a) {	
	windowBackGroundColor.r = r;
	windowBackGroundColor.g = g;
	windowBackGroundColor.b = b;
	windowBackGroundColor.a = a;
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}

/*
* Clear the window to the default background color
*/
void SDLInterface::clearWindow() {
	SDL_RenderClear(_renderer);
}

/*
* Refresh the window content
*/
void SDLInterface::refreshWindow() {
	SDL_RenderPresent(_renderer);
}

/*
* Get the number of milliseconds since the SDL library initialization
* @return an integer value in milliseconds
*/
int SDLInterface::getCurrentTicks() {
	return (int) SDL_GetTicks();
}

/*
* Delay the program during a time
* @param msecs is the number of miliseconds
*/
void SDLInterface::delayEvent(int timeMsec) {
	SDL_Delay(timeMsec);

}

/**
Push the exit event into the system
*/
void SDLInterface::pushExitGameEvent() {
	SDL_Event quit;
	quit.type = SDL_QUIT;
	SDL_PushEvent(&quit);
}

/*
* Detect the input events
*/
void SDLInterface::detectInputEvents() {
	_inputManager.update();
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_WINDOWEVENT_CLOSE:
		case SDL_QUIT:
			_inputManager.pressKey(SDLK_ESCAPE);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}
}

/* Return if the key has been pressed
* @param keyID is the key
* @return true if the key has been pressed. Otherwise, it returns false
*/
bool SDLInterface::isKeyPressed(unsigned int keyID) {
	return _inputManager.isKeyPressed(keyID);
}

/* Return if the key is held down
* @param keyID is the key
* @return true if it is held down. Otherwise, it returns false
*/
bool SDLInterface::isKeyDown(unsigned int keyID) {
	return _inputManager.isKeyDown(keyID);
}

/*
* Get the current screen coordiates of the mouse
* @return a 2D vector with the (x,y) screen coordinates
*/
glm::ivec2 SDLInterface::getMouseCoords() {
	return _inputManager.getMouseCoords();
}

/*
* Get the image dimensions associated to the specified texture
* @param spriteID is the sprite
* @return textureWidth is the width size
* @return textureHeight is the height size
*/
void SDLInterface::getTextureDimension(int textureId, int & textureWidth, int & textureHeight) {
	SDL_QueryTexture(textures[textureId], NULL, NULL, &textureWidth, &textureHeight);
}

/*
* Draw the texture based on the specified parameters
* @param textureId is the texture
* @param (sourceX, sourceY) is the starting position from the texture
* @param (sourceWidth, sourceHeight) is the size of the chunk from the original texture
* @param (targetX, targetY) is the starting position from the texture
* @param (targetWidth, targetHeight) is the size of the chunk from the window where the texture will be copied
*/
void SDLInterface::drawTexture(int textureId, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int targetX, int targetY, int targetWidth, int targetHeight) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = sourceX;
	srcRect.y = sourceY;
	srcRect.w = sourceWidth;
	srcRect.h = sourceHeight;
	destRect.x = targetX;
	destRect.y = targetY;
	destRect.w = targetWidth;
	destRect.h = targetHeight;	
	SDL_RenderCopy(_renderer, textures[textureId], &srcRect, &destRect);
}

/*
* Draw the texture based on the specified parameters and applying a flipping operation
* @param textureId is the texture
* @param (sourceX, sourceY) is the starting position from the texture
* @param (sourceWidth, sourceHeight) is the size of the chunk from the original texture
* @param (targetX, targetY) is the starting position from the texture
* @param (targetWidth, targetHeight) is the size of the chunk from the window where the texture will be copied
* @param flip specifies the flipping operation: SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL
* @param angle (in degrees) is the rotation that will be applied to the texture with respect to its center position ( targetWidth.w / 2, targetWidth.h / 2 )
*/
void SDLInterface::drawFlippedTexture(int textureId, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int targetX, int targetY, int targetWidth, int targetHeight, SDL_RendererFlip flip, float angle) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = sourceX;
	srcRect.y = sourceY;
	srcRect.w = sourceWidth;
	srcRect.h = sourceHeight;
	destRect.x = targetX;
	destRect.y = targetY;
	destRect.w = targetWidth;
	destRect.h = targetHeight;
		//By the default the rotation will be done around dstrect.w / 2, dstrect.h / 2) because the parameter fourth parameter is 0
	SDL_RenderCopyEx(_renderer, textures[textureId], &srcRect, &destRect, angle, 0, flip);
}

/*
* Load a texture and bind it to the specified textureId
* @param textureId identifies the texture
* @param fileName is the texture's location
*/
void SDLInterface::loadTexture(int textureId, std::string fileName) {		
	if (textureId > GAME_TEXTURES) {
		ErrorManagement::errorRunTime("[SDLInterface] You must update the GAME_TEXTURE constants from GameConstants.h because the position that you are requestion is no available");
	}

	//IMG_load from SDL_image extension allow to load many different image formats. The default SDL method called SDL_LoadBMP(...) only works with bmp files
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
	if (pTempSurface == 0) {
		ErrorManagement::errorRunTime("[SDLInterface] Impossible to load the texture called " + fileName);
	}
		//SDL_Texture is better than SDL_Surface because SDL_Texture can be rendered by the GPU
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(_renderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);

	//Add the texture to our list
	if (pTexture != 0) {
		textures[textureId] = pTexture;
	}
	else {
		ErrorManagement::errorRunTime("[SDLInterface] There is a problem in the texture conversion of: " + fileName);
	}
}

/* Draw a colored point at (x,y)
* @param color is the indexed color
* @param (x, y) is the point coordinate
*/
void SDLInterface::drawPoint(int color, int x, int y) {
	drawPoint(colorPalette[color].r, colorPalette[color].g, colorPalette[color].b, colorPalette[color].a, x, y);
}

/* Draw a (r,g,b,a) point at (x,y)
* @param (r, g, b, a) is the RGB color
* @param (x, y) is the point coordinate
*/
void SDLInterface::drawPoint(int r, int g, int b, int a, int x, int y) {
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	SDL_RenderDrawPoint(_renderer, x, y);
	SDL_SetRenderDrawColor(_renderer, windowBackGroundColor.r, windowBackGroundColor.g, windowBackGroundColor.b, windowBackGroundColor.a);
}

/* Draw a (r,g,b,a) line from (x1,y1) to (x2, y2)
* @param color is the indexed color
* @param (x1, y1) is the source coordinate
* @param (x2, y2) is the target coordinate
*/
void SDLInterface::drawLine(int color, int x1, int y1, int x2, int y2) {
	drawLine(colorPalette[color].r, colorPalette[color].g, colorPalette[color].b, colorPalette[color].a, x1, y1, x2, y2);
}

/* Draw a (r,g,b,a) line from (x1,y1) to (x2, y2)
* @param (r, g, b, a) is the RGB color
* @param (x1, y1) is the source coordinate
* @param (x2, y2) is the target coordinate
*/
void SDLInterface::drawLine(int r, int g, int b, int a, int x1, int y1, int x2, int y2) {
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(_renderer, windowBackGroundColor.r, windowBackGroundColor.g, windowBackGroundColor.b, windowBackGroundColor.a);
}


/* Draw a colored rectangle at (x,y) with the width x height size
* @param color is the indexed color
* @param (x, y) is the reference position
* @param width is the rectangle width
* @param height is the rectangle height
*/
void SDLInterface::drawRectangle(int color, int x, int y, int width, int height) {
	drawRectangle(colorPalette[color].r, colorPalette[color].g, colorPalette[color].b, colorPalette[color].a,x, y, width, height);
}

/* Draw a (r,g,b,a) rectangle at (x,y) with the width x height size
* @param (x, y) is the reference position
* @param width is the rectangle width
* @param height is the rectangle height
* @param (r, g, b, a) is the RGB color
*/
void SDLInterface::drawRectangle(int r, int g, int b, int a, int x, int y, int width, int height) {
	SDL_Rect rectangle;
	rectangle.x = x;
	rectangle.y = y;
	rectangle.w = width;
	rectangle.h = height;

	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	SDL_RenderDrawRect(_renderer, &rectangle);
	SDL_SetRenderDrawColor(_renderer, windowBackGroundColor.r, windowBackGroundColor.g, windowBackGroundColor.b, windowBackGroundColor.a);
}

/* Draw a filled rectangle in color at (x,y) with the width x height size
* @param color is the indexted color
* @param (x, y) is the reference position
* @param width is the rectangle width
* @param height is the rectangle height
*/
void SDLInterface::drawFilledRectangle(int color, int x, int y, int width, int height) {
	drawFilledRectangle(colorPalette[color].r, colorPalette[color].g, colorPalette[color].b, colorPalette[color].a, x, y, width, height);
}

/* Draw a (r,g,b,a) filled rectangle at (x,y) with the width x height size
* @param (x, y) is the reference position
* @param width is the rectangle width
* @param height is the rectangle height
* @param (r, g, b, a) is the RGB color
*/
void SDLInterface::drawFilledRectangle(int r, int g, int b, int a, int x, int y, int width, int height) {
	SDL_Rect rectangle;
	rectangle.x = x;
	rectangle.y = y;
	rectangle.w = width;
	rectangle.h = height;

	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	SDL_RenderFillRect(_renderer, &rectangle);
	SDL_SetRenderDrawColor(_renderer, windowBackGroundColor.r, windowBackGroundColor.g, windowBackGroundColor.b, windowBackGroundColor.a);
}

/*
* Set the font style
* @param style is TTF_STYLE_NORMAL, TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE or TTF_STYLE_STRIKETHROUGH
*/
void SDLInterface::setFontStyle(int style) {
	TTF_SetFontStyle(g_font, TTF_STYLE_NORMAL);
}


/*
* Draw a text at screen
* @param text is the text content
* @param backgroundColor is the indexed color for the background
* @param foregroundColor is the indexed color for the foregroundColor
* @param (x, y) is the reference position
*/
void SDLInterface::drawText(std::string text, int backgroundColor, int foregroundColor, int x, int y) {		
	SDL_Surface* textSurface;
	
	textSurface = TTF_RenderText_Shaded(g_font, text.c_str(), colorPalette[backgroundColor], colorPalette[foregroundColor]);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
	
	SDL_Rect TextLocation;
	TextLocation.h = textSurface->h;
	TextLocation.w = textSurface->w;
	TextLocation.x = x;
	TextLocation.y = y;

	SDL_FreeSurface(textSurface);

	SDL_RenderCopy(_renderer, textTexture, NULL, &TextLocation);

	SDL_DestroyTexture(textTexture);
}


/*
* Draw and blend a text at screen
* @param text is the text content
* @param foregroundColor is the indexed color for the foregroundColor
* @param (x, y) is the reference position
*/
void SDLInterface::drawText(std::string text, int foregroundColor, int x, int y) {
	SDL_Surface* textSurface;

	textSurface = TTF_RenderText_Blended(g_font, text.c_str(), colorPalette[foregroundColor]);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

	SDL_Rect TextLocation;
	TextLocation.h = textSurface->h;
	TextLocation.w = textSurface->w;
	TextLocation.x = x;
	TextLocation.y = y;

	SDL_FreeSurface(textSurface);

	SDL_RenderCopy(_renderer, textTexture, NULL, &TextLocation);

	SDL_DestroyTexture(textTexture);
}
