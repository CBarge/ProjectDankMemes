#pragma once

#include <basic_SDL_engi/Window.h>
#include <basic_SDL_engi/GLSLProgram.h>
#include <basic_SDL_engi/Camera2D.h>
#include <basic_SDL_engi/InputManager.h>
#include <basic_SDL_engi/SpriteBatch.h>

#include "Player.h"
#include "Level.h"
#include "Chaser.h"
#include "Button.h"

//for finding out what is going on, man
enum class GameState{ PLAYING, EXIT, MAINMENU, SETTINGS, NEXTLEVEL, LOSER, LEADERBOARD, CREDITS, LOOPING, DEAD };

class MainGame
{
public:
	//constructor
	MainGame();
	//deconstructor
	~MainGame();

	//runs the game, probably important
	void run();

private:
	//initializes stuff like the engine, window, shaders, etc
	void initSystems();

	void initLevels();

	//creation of the level environment
	void initLevel(int numLevel);

	//initializes shaders, called by initSystems
	void initShaders();

	//the main loop that makes stuff actually happen, yo.
	void gameLoop();

	//updates agents, basically the objects in the game. stuff like position updates and what not.
	void updateAgents(float deltaTime);

	//whether or not they are at end of level
	void checkWin();

	//handles all input processes. remember that controls are in player class
	void processInput();

	//draws the game to the window
	void drawGame();

	/// Member Variables
	basic_SDL_engi::Window _window;					//the window the magic happens on
	basic_SDL_engi::GLSLProgram _textureProgram;	//for the shader. textures and stuff.
	basic_SDL_engi::InputManager _inputManager;		//for handling input
	basic_SDL_engi::Camera2D _camera;				//main camera environment
	basic_SDL_engi::SpriteBatch _buttonSpriteBatch;	//draws all of the buttons
	basic_SDL_engi::SpriteBatch _agentSpriteBatch;	//draws all of the agents

	std::vector<Level*> _levels;		//where the action happens.

	//std::vector<Level*> _levels; //placeholder for idea, if it doesn't work take it out

	int _screenWidth, _screenHeight;
	float _fps;

	int _currentLevel;		//idea placeholder

	Player* _player;				//playa
	std::vector<Chaser*> _chasers;	//vector of chasers
	std::vector<Button*> _buttons;	//vector of buttons
	Button* _startButton;
	Button* _exitButton;

	int _score = 0;				//how far you've gotten
	int _numCurrentLevel;
	float _timeSurvived;	//time passed? optional

	GameState _gameState;
	GameState _loopState;
	GameState _nextState;
};