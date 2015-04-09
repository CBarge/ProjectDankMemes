#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace basic_SDL_engi
{

	/*lethalError is triggered when something in the program goes
	terribly wrong. Accepts a string errorString that will print the
	relevant error. Make sure when declaring error possibilities you
	name them properly!*/
	void lethalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter a key to terminate...";
		int stopper;
		std::cin >> stopper;
		SDL_Quit();		//terminating SDL
		exit(1);		//terminating program
	}

}