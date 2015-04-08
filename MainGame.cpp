#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

MainGame::MainGame() :
	_screenWidth(1024), _screenHeight(768), 
	_time(0.0f), 
	_window(nullptr), 
	_gameState(GameState::PLAYING)
{

}


MainGame::~MainGame()
{
	//DEEEEEEEEEEEEEEEEEESTRUCTION!
}

//what actually runs the game!
void MainGame::run()
{
	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);	//temporary!!!

	_playerTexture = ImageLoader::loadPNG("Textures/CharacterRight_Standing.png");

	gameLoop();		//won't return until called to exit using GameState, keep in mind
}

void MainGame::initSystems()
{
	//Initializing SDL, do NOT touch this
	SDL_Init(SDL_INIT_EVERYTHING);

	//window creation! SDL function takes: (window name, xpos, ypos, scrnwidth, scrnheight, flags)
	//for those working with settings, look into SDL flags, there is significant documentation on this!
	_window = SDL_CreateWindow("GraphTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	//making sure window is creatable
	if (_window == nullptr)
	{
		lethalError("SDL window unable to create!");
	}

	//making context, making sure not null
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		lethalError("SDL_GL context unable to create!");
	}

	//initializing glew, setting up extensions
	//glewExperimental = true;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		lethalError("glew unable to initialize!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//double window draw, prevents flicker

	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);		//background color, rgb alpha, use floats

	initShaders();
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
		processInput();
		_time += 0.01f;
		drawGame();
	}
}


/*processInput allows interaction from user, we'll be using SDL's libraries for this
Look up relevant function names please~   ...mainly SDL_Event event types*/
void MainGame::processInput()
{
	SDL_Event eventer;	//"event" is reserved. don't try to use it.

	while (SDL_PollEvent(&eventer))	//basically, while something is happening, do such and such
	{
		//giant switch statement for input consequences!
		switch (eventer.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << eventer.motion.x << " " << eventer.motion.y << std::endl;
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
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("sampleTex");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);		//sends last draw to screen
}