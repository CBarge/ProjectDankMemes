#include "MainGame.h"

#include <basic_SDL_engi/basic_SDL_engi.h>
#include <basic_SDL_engi/Time.h>
#include <basic_SDL_engi/Errors.h>
#include <SDL/SDL.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

const float OBSTACLE_SPEED = 5.0f;
const float PLAYER_SPEED = 11.0f;
const float CHASER_SPEED = 10.0f;

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAYING),
	_fps(0),
	_player(nullptr),
	_score(0),
	_timeSurvived(0.0f)
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
}

void MainGame::run() {
	//for actually creating junk
	initSystems();
	//for actually creating the level
	initLevel();
	//for actually doing anything in the game
	gameLoop();
}

void MainGame::initSystems() {
	//initializing the game engine
	basic_SDL_engi::init();
	//creating the window TO THE FUTURE
	_window.createWin("Runner Game", _screenWidth, _screenHeight, 0);
	//background color, can change?
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	//gettin those shaders so youse can sees the junk
	initShaders();
	//gettin those sprites so there's actually junk to see
	_agentSpriteBatch.init();
	//camera, focused on center of screen.
	_camera.init(_screenWidth, _screenHeight);
}

void MainGame::initLevel() {
	//level environment, could make more levels under this structure?
	// Level 1
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera);

	const std::vector<glm::vec2>& chaserPositions = _levels[_currentLevel]->getChaserPositions();
	for (int i = 0; i < chaserPositions.size(); i++)
	{
		_chasers.push_back(new Chaser);
		_chasers.back()->init(CHASER_SPEED, chaserPositions[i]);
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

	// Main loop
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

		_fps = fpsLimiter.end();
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

	// Update Zombie collisions
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
	// Dont forget to update zombies
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

		// Easy way to end the game :P
		basic_SDL_engi::lethalError("");
	}
	else if (_player->win())
	{
		std::printf("Loser!\nYou got %d points.\n",
			_score);

		// Easy way to end the game :P
		basic_SDL_engi::lethalError("");
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	const float SCALE_SPEED = 0.1f;
	//keeps looping till no more events
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
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
	if (_inputManager.keyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.keyPressed(SDLK_e))
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

	// Unbind the program
	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.bufferSwap();
}