#include "InputManager.h"

/* 
* Default constructor
*/
InputManager::InputManager() :_mouseCoords(0) {
}

/*
* Destructor
*/
InputManager::~InputManager() 	{
}

/*
* Copy the current key states to a data structure that contains the previous states
*/
void InputManager::update() {
		//Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
		//C++ 11 introduces the concept "for each loop" on data structures based on iterators that has begin and end
	for (auto & it : _keyMap) {
		//Copy in the position of the key (it->first), the value (it->second)
		_previousKeyMap[it.first] = it.second;
	}
	//for (auto it = _keyMap.begin(); it != _keyMap.end(); it++) {
	//	_previousKeyMap[it->first] = it->second;
	//}		
}

/*
* Return true if the key was down in the previous frame
* @param keyID is the key
* @return true if it was down. Otherwise, it returns false
*/
bool InputManager::wasKeyDown(unsigned int keyID) {
	// We don't want to use the associative array approach here
	// because we don't want to create a key if it doesn't exist.
	// So we do it manually
	auto it = _previousKeyMap.find(keyID);
	if (it != _previousKeyMap.end()) {
		// Found the key
		return it->second;
	}
	else {
		// Didn't find the key
		return false;
	}
}

/*
* Mark the key as pressed
* @param keyID is the key
*/
void InputManager::pressKey(unsigned int keyID) {
	// Here we are treating _keyMap as an associative array.
	// if keyID doesn't already exist in _keyMap, it will get added
	_keyMap[keyID] = true;
}

/*
* Mark the key as released
* @param keyID is the key
*/
void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

/* 
* Set the mouse coordinates
* @param x is the x's screen position
* @param y is the y's screen position
*/
void InputManager::setMouseCoords(int x, int y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

/* Returns if the key is held down
* @param keyID is the key
* @return true if it is held down. Otherwise, it returns false
*/
bool InputManager::isKeyDown(unsigned int keyID) {
	// We don't want to use the associative array approach here
	// because we don't want to create a key if it doesnt exist.
	// So we do it manually
	auto it = _keyMap.find(keyID);
	if (it != _keyMap.end()) {
		// Found the key
		return it->second;
	} else {
		// Didn't find the key
		return false;
	}
}

/*
* Returns if the key was just pressed
* @param keyID is the key
* @return true if it was just pressed. Otherwise, it returns false
*/
bool InputManager::isKeyPressed(unsigned int keyID) {
		//Check if it was pressed current frame and not last frame
	if ((isKeyDown(keyID)) && (wasKeyDown(keyID) == false)) {
		return true;
	}
	return false;
}	

