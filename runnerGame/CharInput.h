#include <SDL/SDL.h>

#include <basic_SDL_engi/InputManager.h>

class CharInput{
public:
	CharInput();
	~CharInput();

	int charCount; //tracks number of chars
	
	char getChar();

private:
	basic_SDL_engi::InputManager* _inputManager;
	
};