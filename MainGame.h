#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

#include <SDL/SDL.h>
//if we are making this multiplat, the opengl call will change! keep this in mind!
#include <GL/glew.h>

#include "GLSLProgram.h"
#include "GLTexture.h"
#include "Sprite.h"

//for finding out what is going on, man
enum class GameState{PLAYING, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	SDL_Window* _window;	//window creation, private FOR A REASON!!!
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	//temporary! for testing!
	Sprite _sprite;
	GLSLProgram _colorProgram;
	GLTexture _playerTexture;

	float _time;
};

