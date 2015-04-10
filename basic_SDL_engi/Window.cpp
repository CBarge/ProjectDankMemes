#include "Window.h"
#include "Errors.h"

namespace basic_SDL_engi
{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::createWin(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISI)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//window creation! SDL function takes: (window name, xpos, ypos, scrnwidth, scrnheight, flags)
		//for those working with settings, look into SDL flags, there is significant documentation on this!
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		//making sure window is creatable
		if (_sdlWindow == nullptr)
		{
			lethalError("SDL window unable to create!");
		}

		//making context, making sure not null
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
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

		//check OpenGL version
		std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

		glClearColor(0.1f, 0.1f, 0.4f, 1.0f);		//background color, rgb alpha, use floats

		//enabling vsync...consensus needed on this. can be a settings option!
		SDL_GL_SetSwapInterval(1);

		//Enable alpha blending...
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::bufferSwap()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}

}