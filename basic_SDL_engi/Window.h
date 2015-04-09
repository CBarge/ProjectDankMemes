#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace basic_SDL_engi
{

	//CONSIDER OTHER FLAGS!
	//Windows position, type, etc
	enum WindowFlags { INVISI = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };	//set for bitwise passing!

	class Window
	{
	public:
		Window();
		~Window();

		int createWin(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);	//use bitwise when passing!

		void bufferSwap();

		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }


	private:
		SDL_Window* _sdlWindow;
		int _screenWidth, _screenHeight;
	};

}