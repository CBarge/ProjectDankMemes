#include "MainGame.h"
#include <basic_SDL_engi/Errors.h>
#include <basic_SDL_engi/ResourceManager.h>

MainGame::MainGame() :
	_screenWidth(1024), _screenHeight(768), 
	_time(0.0f), 
	_gameState(GameState::PLAYING),
	_maxFPS(60.0f)
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
		float startTicks = SDL_GetTicks();
		processInput();
		_time += 0.01f;

		_camera.update();

		drawGame();
		calcFPS();

		static int fCount = 0;
		fCount++;
		if (fCount == 10)
		{
			std::cout << _fps << std::endl;
			fCount = 0;
		}
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);		//8ms delay, OS specific!
		}
	}
}


/*processInput allows interaction from user, we'll be using SDL's libraries for this
Look up relevant function names please~   ...mainly SDL_Event event types*/
void MainGame::processInput()
{
	SDL_Event eventer;	//"event" is reserved. don't try to use it.

	const float CAM_SPEED = 20.0f;
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
			break;
		case SDL_KEYDOWN:
			switch (eventer.key.keysym.sym)
			{
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAM_SPEED));
				break;
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAM_SPEED));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-CAM_SPEED, 0.0f));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(CAM_SPEED, 0.0f));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
				break;
			}
		}
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

	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//setting camera matrix...
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	//will later have an object to pass, this is placeholder
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static basic_SDL_engi::GLTexture texture = basic_SDL_engi::ResourceManager::getTexture("Textures/TubaCannon.png");
	basic_SDL_engi::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	//needs position, uv, and depth
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.bufferSwap();		//sends last draw to screen
}

void MainGame::calcFPS()
{
	static const int SAMPLE_NUM = 10;
	static float frameTimes[SAMPLE_NUM];
	static int frame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[frame % SAMPLE_NUM] = _frameTime;

	prevTicks = currentTicks;

	int count;

	frame++;
	if (frame < SAMPLE_NUM)
	{
		count = frame;
	}
	else
	{
		count = SAMPLE_NUM;
	}

	float frameTimeAvg = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAvg += frameTimes[i];
	}
	frameTimeAvg /= count;

	if (frameTimeAvg > 0)
	{
		_fps = 1000.0f / frameTimeAvg;
	}
	else
	{
		_fps = 60.0f;
	}
}