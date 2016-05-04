#pragma once

/* 
* The Character class represents a character of the game
*/

class Sprite {
	int _xAtWorld;
	int _yAtWorld;	
	int _textureID;	
	int _currentFrame;
	int _maxFrames;

public:
	Sprite();
	~Sprite();

		//Getter and setter methods
	void setInitialValues(int x, int y, int textureID, int currentFrame, int maxFrames);
	void setPositionAtWorld(int newXPosition, int newYPosition);
	int getXAtWorld();
	int getYAtWorld();
	int getSpriteId();
	int getMaxFrames();
	void setCurrentFrame(int newCurrentFrame);
	int getCurrentFrame();
	void nextFrame(int nextValue);
	
	
};

