#include "MainGame.h"
#include <basic_SDL_engi/Errors.h>
#include <basic_SDL_engi/ResourceManager.h>

MainGame::MainGame() :
	_screenWidth(1024), _screenHeight(768), 
	_time(0.0f), 
	_gameState(GameState::PLAYING),
	_max(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
	//DEEEEEEEEEEEEEEEEEESTRUCTION!
}

//what actually runs the game!
void MainGame::run()
{
	initSystems();


	gameLoop();		//won't return until called to exit using GameState, keep in mind
}

void MainGame::initSystems()
{
	basic_SDL_engi::init();

	_window.createWin("GAME FOR COOL KIDS", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_max);
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vrt", "Shaders/colorShading.frg");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

/*gameLoop makes the game's heart go tick tock
basically allows the program to persist while doing stuff*/
void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		_inputManager.update();

		processInput();
		_time += 0.01f;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();

		static int fCount = 0;
		fCount++;
		if (fCount == 1000)
		{
			std::cout << _fps << std::endl;
			fCount = 0;
		}
	}
}


/*processInput allows interaction from user, we'll be using SDL's libraries for this
Look up relevant function names please~   ...mainly SDL_Event event types*/
void MainGame::processInput()
{
	SDL_Event eventer;	//"event" is reserved. don't try to use it.

	const float CAM_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&eventer))	//basically, while something is happening, do such and such
	{
		//giant switch statement for input consequences!
		switch (eventer.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << eventer.motion.x << " " << eventer.motion.y << std::endl;
			_inputManager.setCursorCoords(eventer.motion.x, eventer.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.onPress(eventer.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.onRelease(eventer.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.onPress(eventer.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.onRelease(eventer.button.button);
			break;
		}
	}

	if (_inputManager.keyDown(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAM_SPEED));
	}
	if (_inputManager.keyDown(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAM_SPEED));
	}
	if (_inputManager.keyDown(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAM_SPEED, 0.0f));
	}
	if (_inputManager.keyDown(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAM_SPEED, 0.0f));
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

void MainGame::drawGame()
{
	//clear depth setting
	glClearDepth(1.0);
	//clearing buffer, graphics
	//keep in mind bitwise operations! very helpful!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("sampleTex");
	glUniform1i(textureLocation, 0);

	//setting camera matrix...
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	//will later have an object to pass, this is placeholder
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static basic_SDL_engi::GLTexture texture = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_duck.png");
	basic_SDL_engi::Color color(255, 255, 255, 255);
	//needs position, uv, and depth
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.bufferSwap();		//sends last draw to screen
}
