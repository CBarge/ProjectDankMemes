#include "MainGame.h"

#include <basic_SDL_engi/basic_SDL_engi.h>
#include <basic_SDL_engi/Time.h>
#include <basic_SDL_engi/Errors.h>
#include <SDL/SDL.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>
#include <string>

const float OBSTACLE_SPEED = 5.0f;
const float PLAYER_SPEED = 10.1f;
const float CHASER_SPEED = 10.0f;

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::MAINMENU),
	_loopState(GameState::LOOPING),
	_nextState(GameState::MAINMENU),
	_fps(0),
	_player(nullptr),
	_score(0),
	_timeSurvived(0.0f),
	_numCurrentLevel(0)
{
	//we really shouldn't need anything here
}

MainGame::~MainGame() {
	//removing level data
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	//removing obstacle data
	for (int i = 0; i < _chasers.size(); i++) {
		delete _chasers[i];
	}
	delete _player;
	//clearing music and sfx
	Music::clearMusic();
	Music::closePlayer();
	SFX::clearFX();
}

void MainGame::run() {
	//for actually creating junk
	initSystems();
	//getting all level data
	initLevels();
	//for actually creating the level
	initLevel(0);
	//for actually doing anything in the game
	gameLoop();
}

void MainGame::initSystems() {
	//initializing the game engine
	basic_SDL_engi::init();
	//creating the window TO THE FUTURE
	_window.createWin("Runner Game", _screenWidth, _screenHeight, 0);
	//background color, can change?
	glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
	//gettin those shaders so youse can sees the junk
	initShaders();
	//gettin those sprites so there's actually junk to see
	_agentSpriteBatch.init();
	//for those coolio doolio buttons, mang
	_buttonSpriteBatch.init();
	//camera, focused on center of screen.
	_camera.init(_screenWidth, _screenHeight);
}

void MainGame::initLevels()
{
	_levels.push_back(new Level("Levels/level0.txt"));
	_levels.push_back(new Level("Levels/level1.txt"));
	_levels.push_back(new Level("Levels/level2.txt"));
}

void MainGame::initLevel(int numLevel) {
	//level environment, could make more levels under this structure?
	//zeroth level, nothing happens
	if (numLevel == 0)
	{
		// Level 0
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //main menu background is black
		_currentLevel = 0;
		_numCurrentLevel = 0;
	}
	else if (numLevel == 1)
	{
		// Level 1
		_currentLevel = 1;
		_numCurrentLevel = 1;

		_player = new Player();
		_player->init(PLAYER_SPEED, _levels[1]->getStartPlayerPos(), &_inputManager, &_camera);

		const std::vector<glm::vec2>& chaserPositions = _levels[_currentLevel]->getChaserPositions();
		_chasers.clear();
		for (int i = 0; i < chaserPositions.size(); i++)
		{
			_chasers.push_back(new Chaser);
			_chasers.back()->init(CHASER_SPEED, chaserPositions[i]);
		}
	}
	else if (numLevel == 2)
	{
		// Level 2
		_currentLevel = 2;
		_numCurrentLevel = 2;

		_player = new Player();
		_player->init(PLAYER_SPEED, _levels[2]->getStartPlayerPos(), &_inputManager, &_camera);

		const std::vector<glm::vec2>& chaserPositions = _levels[_currentLevel]->getChaserPositions();
		for (int i = 0; i < chaserPositions.size(); i++)
		{
			_chasers.push_back(new Chaser);
			_chasers.back()->init(CHASER_SPEED, chaserPositions[i]);
		}
	}

}

void MainGame::initShaders() {
	//gettin those shaders in there boy
	_textureProgram.compileShaders("Shaders/colorShading.vrt", "Shaders/colorShading.frg");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 5;

	basic_SDL_engi::FPSLimiter fpsLimiter;
	fpsLimiter.setFPS(60.0f);

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / 60;	//# of milliseconds to render one frame
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks();

	while (_loopState == GameState::LOOPING)
	{
		//Main Menu Loop
		if (_gameState == GameState::MAINMENU)
		{
			initLevel(0);
			Music::playMusic(1);
			_startButton = new Button;
			_startButton->init(0, -50.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_exitButton = new Button;
			_exitButton->init(1, -50.0f, -100.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_leaderButton = new Button;
			_leaderButton->init(6, -384.0f, -487.0f, 50.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::MAINMENU) {
				fpsLimiter.begin();

				float newTicks = SDL_GetTicks();
				float frameTime = SDL_GetTicks() - prevTicks;
				prevTicks = newTicks;
				float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

				processInput();

				glm::vec2 origin = glm::vec2(0.0f, 0.0f);

				_camera.setPosition(origin);

				_camera.update();


				if (_startButton->checkPressed())
				{
					SFX::playSound(6);
					_nextState = GameState::PLAYING;
					_currentLevel = 1;
					std::cout << "Butts\n";
				}
				else if (_exitButton->checkPressed())
				{
					SFX::playSound(6);
					std::cout << "Barf\n";
					_nextState = GameState::EXIT;
					_loopState = GameState::DEAD;
				}
				else if (_leaderButton->checkPressed())
				{
					SFX::playSound(6);

					_nextState = GameState::LEADERBOARD;
				}
				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
			}
		}

		//Settings Menu Loop
		else if (_gameState == GameState::SETTINGS)
		{
			while (_gameState == GameState::SETTINGS) {

			}
		}

		// Main Game loop
		else if (_gameState == GameState::PLAYING)
		{
			Music::playMusic(0);
			initLevel(1);
			while (_gameState == GameState::PLAYING) {
				fpsLimiter.begin();

				float newTicks = SDL_GetTicks();
				float frameTime = SDL_GetTicks() - prevTicks;
				prevTicks = newTicks;
				float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

				checkWin();

				processInput();

				int i = 0;

				while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
				{
					float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
					updateAgents(deltaTime);
					totalDeltaTime -= deltaTime;
					i++;
				}

				_camera.setPosition(_player->getPosition());

				_camera.update();

				_inputManager.update();

				drawGame();

				_gameState = _nextState;

				_fps = fpsLimiter.end();
			}
		}

		//Losing Screen Loop
		else if (_gameState == GameState::LOSER)
		{
			while (_gameState == GameState::LOSER) {
				Music::stopMusic(1); //stop music first or play fail sound while music still playing?
				SFX::playSound(0);
				_nextState = GameState::MAINMENU;
				_gameState = _nextState;
			}
		}

		//New high score loop
		else if (_gameState == GameState::HIGHSCORE)
		{
			std::string name = "";
			_newName->charCount = 0; //resets charCount to 0
			while (_gameState == GameState::HIGHSCORE){

				processInput();
				//enters loop to wait for 3 char input
				while (_newName->charCount < 3 && _newName->getChar() != '1'){
					name += _newName->getChar(); //appends to name
				}
				//need to save name to leaderboard file here by calling Leaderboard function, i'll do that
				//once name is entered, changes to leaderboard screen
				_nextState = GameState::LEADERBOARD;

			}
		}
		//Leaderboard Loop
		else if (_gameState == GameState::LEADERBOARD)
		{
			while (_gameState == GameState::LEADERBOARD) {

			}
		}

		//Credits Loop
		else if (_gameState == GameState::CREDITS)
		{
			while (_gameState == GameState::CREDITS) {

			}
		}
	}
	
}

void MainGame::updateAgents(float deltaTime) {

	//update player
	_player->update(_levels[_currentLevel]->getLevelData(), _player, _chasers, deltaTime);

	// Update all chasers
	for (int i = 0; i < _chasers.size(); i++)
	{
		_chasers[i]->update(_levels[_currentLevel]->getLevelData(), _player, _chasers, deltaTime);
	}

	// Update chasers collisions
	for (int i = 0; i < _chasers.size(); i++)
	{
		// Collide with other zombies
		for (int j = i + 1; j < _chasers.size(); j++)
		{
			_chasers[i]->collideWithAgent(_chasers[j]);
		}
		// Collide with player
		if (_chasers[i]->collideWithAgent(_player))
		{
			_player->damage(1.0f);
		}
	}

	_score += floor(deltaTime);
}

void MainGame::checkWin() {
	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

	//if you die you're dead
	if (_player->dead())
	{
		// Print failure message
		std::printf("Loser!\nYou got %d points.\n",
			_score);
		_nextState = GameState::LOSER;
		// Easy way to end the game :P
		//basic_SDL_engi::lethalError("");
	}
	else if (_player->win())
	{
		SFX::playSound(0);

		std::printf("Win...\nYou got %d points.\n",
			_score);

		_currentLevel++;
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	const float SCALE_SPEED = 0.1f;
	//keeps looping till no more events
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_loopState = GameState::DEAD;
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setCursorCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.onPress(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.onRelease(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.onPress(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.onRelease(evnt.button.button);
			break;
		}
	}
	if (_inputManager.keyDown(SDLK_q))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.keyDown(SDLK_e))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.keyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 cursorCoords = _inputManager.getCursorCoords();
		cursorCoords = _camera.convSCoordsToWCoords(cursorCoords);
		std::cout << cursorCoords.x << " " << cursorCoords.y << "\n";
	}
}

void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("sampleTex");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//drawing the level
	_levels[_currentLevel]->draw();

	if (_gameState == GameState::MAINMENU)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_startButton->draw(_buttonSpriteBatch);

		_exitButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();
	}
	else if (_gameState == GameState::PLAYING)
	{
		//start drawing the agents
		_agentSpriteBatch.begin();

		//player
		_player->draw(_agentSpriteBatch);

		//chasers
		for (int i = 0; i < _chasers.size(); i++) {
			_chasers[i]->draw(_agentSpriteBatch);
		}

		// End spritebatch creation
		_agentSpriteBatch.end();

		// Render to the screen
		_agentSpriteBatch.renderBatch();
	}

	// Unbind the program
	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.bufferSwap();
}