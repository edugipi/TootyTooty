#pragma once
#include <unordered_map>
#include <glm\glm.hpp>

/*
 * Input manager stores a key map that maps SDL_Keys to booleans.
 * If the value in the key map is true, then the key is pressed.
 * Otherwise, it is released.
 */
class InputManager
{
public:
	InputManager();
	~InputManager();

		//Getter and Setter methods
	void update();
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	void setMouseCoords(int x, int y);
	glm::ivec2 getMouseCoords() { return _mouseCoords; }

		//Check key status
	bool isKeyPressed(unsigned int keyID); 			//Returns true if the key was just pressed	
	bool isKeyDown(unsigned int keyID);				//Returns true if the key is held down

private:
	bool wasKeyDown(unsigned int keyID);
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	std::unordered_map<unsigned int, bool> _keyMap;
	glm::ivec2 _mouseCoords;
};
