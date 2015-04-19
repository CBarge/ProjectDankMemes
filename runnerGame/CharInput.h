#pragma once
#include <SDL/SDL.h>

#include <basic_SDL_engi/InputManager.h>
#include <string>

class CharInput{
public:
	CharInput();
	~CharInput();

	void init(basic_SDL_engi::InputManager* inputManager);

	void getChar();
	//getters
	int size();
	bool done();
	std::string getName();

	void resetName();
private:
	basic_SDL_engi::InputManager* _inputManager;

	bool _done = false;
	std::string _name;
};