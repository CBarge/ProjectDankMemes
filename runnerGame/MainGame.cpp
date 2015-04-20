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
	//initializing sound
	_audio.init();
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
	//gotta have them chunky funky sprites, yo
	_hudSpriteBatch.init();
	//initializing the spritefont
	_spriteFont = new basic_SDL_engi::SpriteFont("Fonts/Iron & Brine.ttf", 32);
	//camera, focused on center of screen.
	_camera.init(_screenWidth, _screenHeight);
	//camera for the hud, "travels"
	_hudCamera.init(_screenWidth, _screenHeight);
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));
	//camera for the hud, origin is bottom left
	_menuCamera.init(_screenWidth, _screenHeight);
	_menuCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

	_leaderboard.initLeaderboard();

	_music = _audio.loadMusic("Music/MenuMusic.wav");
	_music.play(-1);
}

void MainGame::initLevels()
{
	_levels.push_back(new Level("Levels/level0.txt"));
	_levels.push_back(new Level("Levels/level1.txt"));
	_levels.push_back(new Level("Levels/level2.txt"));
	_levels.push_back(new Level("Levels/level3.txt"));
	_levels.push_back(new Level("Levels/level4.txt"));
	_levels.push_back(new Level("Levels/level5.txt"));
}

void MainGame::initLevel(int numLevel) {
	//circles back to level 1
	if (numLevel > 5)
		numLevel = 1;
	//zeroth level, nothing happens
	if (numLevel == 0)
	{
		glClearColor(0.05f, 0.07f, 0.1f, 0.0f); //main menu background is black
		_currentLevel = 0;
	}
	else if (numLevel == 1)
	{
		glClearColor(0.01f, 0.0f, 0.0f, 0.0f);
		_currentLevel = 1;
	}
	else if (numLevel == 2)
	{
		glClearColor(0.03f, 0.0f, 0.0f, 0.0f);
		_currentLevel = 2;
	}
	else if (numLevel == 3)
	{
		glClearColor(0.06f, 0.0f, 0.0f, 0.0f);
		_currentLevel = 3;
	}
	else if (numLevel == 4)
	{
		glClearColor(0.09f, 0.0f, 0.0f, 0.0f);
		_currentLevel = 4;
	}
	else if (numLevel == 5)
	{
		glClearColor(0.14f, 0.0f, 0.0f, 0.0f);
		_currentLevel = 5;
	}
		_player = new Player();
		_player->init(PLAYER_SPEED, _levels[2]->getStartPlayerPos(), &_inputManager, &_camera);

		const std::vector<glm::vec2>& chaserPositions = _levels[_currentLevel]->getChaserPositions();
		_chasers.clear();
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

	while (_loopState == GameState::LOOPING)
	{
		//Main Menu Loop
		if (_gameState == GameState::MAINMENU)
		{
			initLevel(0);
			_score = 0;
			_startButton = new Button;
			_startButton->init(0, -50.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_exitButton = new Button;
			_exitButton->init(1, 125.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_creditsButton = new Button;
			_creditsButton->init(5, -225.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_leaderButton = new Button;
			_leaderButton->init(6, -25.0f, -100.0f, 50.0f, 50.0f, &_inputManager, &_camera);

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

				_hudCamera.update();

				if (_startButton->checkPressed())
				{
					_nextState = GameState::PLAYING;
					_numCurrentLevel = 1;
					_music = _audio.loadMusic("Music/MusicGame.wav");
					_music.play(-1);
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}
				else if (_creditsButton->checkPressed())
				{
					_nextState = GameState::CREDITS;
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}
				else if (_leaderButton->checkPressed())
				{
					_nextState = GameState::LEADERBOARD;
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}
				else if (_exitButton->checkPressed())
				{
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
					_nextState = GameState::EXIT;
					_loopState = GameState::DEAD;
				}

				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
			}
		}

		//nextLevel Menu Loop
		else if (_gameState == GameState::NEXTLEVEL)
		{
			initLevel(0);
			_startButton = new Button;
			_startButton->init(0, -50.0f, -175.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::NEXTLEVEL) {
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
					_nextState = GameState::PLAYING;
					_numCurrentLevel++;
					std::cout << _currentLevel;
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}

				_hudCamera.update();

				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
			}
		}

		// Main Game loop
		else if (_gameState == GameState::PLAYING)
		{
			initLevel(_numCurrentLevel);
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

				_hudCamera.update();

				_inputManager.update();

				drawGame();

				_gameState = _nextState;

				_fps = fpsLimiter.end();
			}
		}

		//Losing Screen Loop
		else if (_gameState == GameState::LOSER)
		{
			initLevel(0);
			_menuButton = new Button;
			_menuButton->init(7, -225.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			_exitButton = new Button;
			_exitButton->init(1, 125.0f, 0.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::LOSER) {
				fpsLimiter.begin();

				float newTicks = SDL_GetTicks();
				float frameTime = SDL_GetTicks() - prevTicks;
				prevTicks = newTicks;
				float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

				processInput();

				glm::vec2 origin = glm::vec2(0.0f, 0.0f);

				_camera.setPosition(origin);
				_camera.update();


				if (_menuButton->checkPressed())
				{
					_nextState = GameState::MAINMENU;
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}
				else if (_exitButton->checkPressed())
				{
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
					_nextState = GameState::EXIT;
					_loopState = GameState::DEAD;
				}

				_hudCamera.update();

				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
			}
		}

		//New high score loop
		else if (_gameState == GameState::HIGHSCORE)
		{
			initLevel(0);
			_newName = new CharInput;
			_newName->init(&_inputManager);
			_exitButton = new Button;
			_exitButton->init(1, -50.0f, -300.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::HIGHSCORE)
			{
				fpsLimiter.begin();

				float newTicks = SDL_GetTicks();
				float frameTime = SDL_GetTicks() - prevTicks;
				prevTicks = newTicks;
				float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

				processInput();
				
				glm::vec2 origin = glm::vec2(0.0f, 0.0f);

				_camera.setPosition(origin);
				_camera.update();

				if (_exitButton->checkPressed())
				{
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
					_nextState = GameState::EXIT;
					_loopState = GameState::DEAD;
				}

				if (!_newName->done())
				{
					_newName->getChar();
				}
				else
				{
					_nextState = GameState::LEADERBOARD;
				}

				_hudCamera.update();

				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
			}
			_leaderboard.addHighScore(_score, _newName->getName());
			_leaderboard.initLeaderboard();
		}

		//Leaderboard Loop
		else if (_gameState == GameState::LEADERBOARD)
		{
			_leaderboard.initLeaderboard();
			initLevel(0);
			_menuButton = new Button;
			_menuButton->init(7, -50.0f, -350.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::LEADERBOARD)
			{
					fpsLimiter.begin();

					float newTicks = SDL_GetTicks();
					float frameTime = SDL_GetTicks() - prevTicks;
					prevTicks = newTicks;
					float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

					processInput();

					glm::vec2 origin = glm::vec2(0.0f, 0.0f);

					_camera.setPosition(origin);
					_camera.update();


					if (_menuButton->checkPressed())
					{
						_nextState = GameState::MAINMENU;
						_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
						_sfx.play(0);
					}

					_hudCamera.update();

					_inputManager.update();

					drawGame();
					_gameState = _nextState;
					_fps = fpsLimiter.end();
			}
		}

		//Credits Loop
		else if (_gameState == GameState::CREDITS)
		{
			initLevel(0);
			_menuButton = new Button;
			_menuButton->init(7, -50.0f, -350.0f, 100.0f, 50.0f, &_inputManager, &_camera);
			while (_gameState == GameState::CREDITS) {
				fpsLimiter.begin();

				float newTicks = SDL_GetTicks();
				float frameTime = SDL_GetTicks() - prevTicks;
				prevTicks = newTicks;
				float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

				processInput();

				glm::vec2 origin = glm::vec2(0.0f, 0.0f);

				_camera.setPosition(origin);
				_camera.update();


				if (_menuButton->checkPressed())
				{
					_nextState = GameState::MAINMENU;
					_sfx = _audio.loadSFX("Sound_FX/menu_navigate.wav");
					_sfx.play(0);
				}

				_hudCamera.update();

				_inputManager.update();

				drawGame();
				_gameState = _nextState;
				_fps = fpsLimiter.end();
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

void MainGame::checkWin()
{
	//if you die you're dead
	if (_player->dead())
	{
		_music = _audio.loadMusic("Music/MenuMusic.wav");
		_music.play(-1);
		if (_leaderboard.checkHighScore(_score))
		{
			_sfx = _audio.loadSFX("Sound_FX/applause.wav");
			_sfx.play(0);
			_nextState = GameState::HIGHSCORE;
		}
		else
		{
			_sfx = _audio.loadSFX("Sound_FX/death_scream.wav");
			_sfx.play(0);
			_nextState = GameState::LOSER;
		}
	}
	else if (_player->win())
	{
		_sfx = _audio.loadSFX("Sound_FX/applause.wav");
		_sfx.play(0);
		_nextState = GameState::NEXTLEVEL;
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
			_loopState = GameState::DEAD;
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

		/*glm::vec4 pos(-400.0f, 120.0f, 800.0f, 150.0f);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		static basic_SDL_engi::GLTexture texture = basic_SDL_engi::ResourceManager::getTexture("Textures/buttLogo.png");
		basic_SDL_engi::Color color(255, 255, 255, 255);

		_buttonSpriteBatch.draw(pos, uv, texture.id, 0.0f, color);
		*/
		_startButton->draw(_buttonSpriteBatch);

		_exitButton->draw(_buttonSpriteBatch);

		_creditsButton->draw(_buttonSpriteBatch);

		_leaderButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
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

		drawHud();
	}
	else if (_gameState == GameState::NEXTLEVEL)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_startButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
	}
	else if (_gameState == GameState::LOSER)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_menuButton->draw(_buttonSpriteBatch);

		_exitButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
	}
	else if (_gameState == GameState::HIGHSCORE)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_exitButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
	}
	else if (_gameState == GameState::LEADERBOARD)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_menuButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
	}
	else if (_gameState == GameState::CREDITS)
	{
		//start drawing the agents
		_buttonSpriteBatch.begin();

		_menuButton->draw(_buttonSpriteBatch);

		// End spritebatch creation
		_buttonSpriteBatch.end();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		// Render to the screen
		_buttonSpriteBatch.renderBatch();

		drawText();
	}

	// Unbind the program
	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.bufferSwap();
}


void MainGame::drawHud()
{
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();

	sprintf_s(buffer, "HEALTH: %.0f", _player->health());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(10, 0), glm::vec2(1.1), 0.0f,
		basic_SDL_engi::Color(50, 50, 50, 195), basic_SDL_engi::Justification::LEFT);

	sprintf_s(buffer, "HEALTH: %.0f", _player->health());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(10, 0), glm::vec2(1.0), 0.0f,
		basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::LEFT);

	sprintf_s(buffer, "SCORE: %d", _score);

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth - 10, 0), glm::vec2(1.1), 0.0f,
		basic_SDL_engi::Color(50, 50, 50, 195), basic_SDL_engi::Justification::RIGHT);

	sprintf_s(buffer, "SCORE: %d", _score);

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth - 10, 0), glm::vec2(1.0), 0.0f,
		basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::RIGHT);

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::drawText()
{
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();

	if (_gameState == GameState::MAINMENU)
	{
		sprintf_s(buffer, "BUTT ADVENTURES");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.4), 0.0f,
			basic_SDL_engi::Color(50, 50, 50, 140), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "BUTT ADVENTURES");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.2), 0.0f,
			basic_SDL_engi::Color(50, 50, 50, 195), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "BUTT ADVENTURES");
		
		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
		
		sprintf_s(buffer, "Try to run as far as you can!");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2((_screenWidth / 2), (_screenHeight / 2) - 200), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Don't slow down, you'll probably die or something.");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2((_screenWidth / 2), (_screenHeight / 2) - 230), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Press W to jump, A to slow, and D to speed up.");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2((_screenWidth / 2), (_screenHeight / 2) - 260), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
	}
	else if (_gameState == GameState::NEXTLEVEL)
	{
		sprintf_s(buffer, "LEVEL PASSED!");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Get ready for the next one...");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 2)), glm::vec2(1.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

	}
	else if (_gameState == GameState::LOSER)
	{
		sprintf_s(buffer, "GAME OVER");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.2), 0.0f,
			basic_SDL_engi::Color(50, 50, 50, 195), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "GAME OVER");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 3)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Try not to do whatever you did anymore.");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2((_screenWidth / 2), (_screenHeight / 2) - 150), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "SCORE: %d", _score);

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight / 2), glm::vec2(1.1), 0.0f,
			basic_SDL_engi::Color(50, 50, 50, 195), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "SCORE: %d", _score);

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight / 2), glm::vec2(1.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
	}
	else if (_gameState == GameState::HIGHSCORE)
	{
		sprintf_s(buffer, "HIGH SCORE!");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 6)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
		
		sprintf_s(buffer, "Enter your name: ");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 2)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "%s", _newName->getName().c_str());

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight / 3), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
	}
	else if (_gameState == GameState::LEADERBOARD)
	{
		std::vector<int> scores = _leaderboard.getScores();
		std::vector<std::string> names = _leaderboard.getNames();
		sprintf_s(buffer, "TOP SCORES");
		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 6)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
		for (unsigned int i = 0; i < 10; i++)
		{
			sprintf_s(buffer, "%d. %s ------------ %d", i+1, names[i].c_str(), scores[i]);

			_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - (50 * i)), glm::vec2(1.0), 0.0f,
				basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
		}
	}
	else if (_gameState == GameState::CREDITS)
	{
		sprintf_s(buffer, "Credits");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 6)), glm::vec2(2.0), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Rhenn Handel, Andrea Costa, Arturo Bustamante");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4)), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Logan Barkes, Charlie Davidson");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 30), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Pheobe Chu, David Honeycutt");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 60), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Menu Music - Up In My Jam by Kubbi");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 120), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Game Music - Seahorse by Kubbi");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 150), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Engine design inspired by an SDL tutorial series by Benjamin Arnold");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 210), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "SpriteFont class by Cristian Zaloj and Benjamin Arnold");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 240), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "picoPNG Copyright (c) 2005-2010 Lode Vandevenne");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 300), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Font is Iron & Brine,");

		_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 360), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

		sprintf_s(buffer, "Copyright(c) 2015 by S.John Ross. \"Cumberland Games & Diversions\"");

			_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 390), glm::vec2(0.9), 0.0f,
			basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);

			sprintf_s(buffer, "Thanks for playing!");

			_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(_screenWidth / 2, _screenHeight - (_screenHeight / 4) - 450), glm::vec2(0.9), 0.0f,
				basic_SDL_engi::Color(255, 255, 255, 255), basic_SDL_engi::Justification::MIDDLE);
	}

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}