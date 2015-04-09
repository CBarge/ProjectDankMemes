#include <SDL/SDL.h>
#include <GL/glew.h>

#include "basic_SDL_engi.h"

namespace basic_SDL_engi
{

	int init()
	{
		//Initializing SDL, do NOT touch this
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//double window draw, prevents flicker

		return 0;
	}

}