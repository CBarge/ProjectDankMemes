#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include <SDL/SDL.h>
//if we are making this multiplat, the opengl call will change! keep this in mind!
#include <GL/glew.h>

#include <basic_SDL_engi/basic_SDL_engi.h>
#include <basic_SDL_engi/GLSLProgram.h>
#include <basic_SDL_engi/GLTexture.h>
#include <basic_SDL_engi/Sprite.h>
#include <basic_SDL_engi/Window.h>
#include <basic_SDL_engi/Camera2D.h>
#include <basic_SDL_engi/SpriteBatch.h>

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
	void calcFPS();

	basic_SDL_engi::Window _window;	//window creation, private FOR A REASON!!!
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	basic_SDL_engi::GLSLProgram _colorProgram;
	basic_SDL_engi::Camera2D _camera;
	basic_SDL_engi::SpriteBatch _spriteBatch;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;
};

