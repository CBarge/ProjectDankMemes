#include "InputManager.h"

namespace basic_SDL_engi
{

	InputManager::InputManager() : _cursorCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
		{
			_prevKeyMap[it->first] = it->second;
		}
	}

	void InputManager::onPress(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void InputManager::onRelease(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void InputManager::setCursorCoords(float x, float y)
	{
		_cursorCoords.x = x;
		_cursorCoords.y = y;
	}

	bool InputManager::keyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::keyPressed(unsigned int keyID)
	{
		bool isPressed;
		if (keyDown(keyID) == true && wasKeyDown(keyID) == false)
		{
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = _prevKeyMap.find(keyID);
		if (it != _prevKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}