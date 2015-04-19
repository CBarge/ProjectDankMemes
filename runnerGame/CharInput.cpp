#include "CharInput.h"

CharInput::CharInput()
{
}
CharInput::~CharInput()
{
}

void CharInput::init(basic_SDL_engi::InputManager* inputManager)
{
	_inputManager = inputManager;
}

void CharInput::getChar(){
	if (_inputManager->keyPressed(SDLK_a)){
		_name += 'A';
	}
	else if (_inputManager->keyPressed(SDLK_b)){
		_name += 'B';
		
	}
	else if (_inputManager->keyPressed(SDLK_c)){
		_name += 'C';
		
	}
	else if (_inputManager->keyPressed(SDLK_d)){
		_name += 'D';
		
	}
	else if (_inputManager->keyPressed(SDLK_e)){
		_name += 'E';
		
	}
	else if (_inputManager->keyPressed(SDLK_f)){
		_name += 'F';
		
	}
	else if (_inputManager->keyPressed(SDLK_g)){
		_name += 'G';
		
	}
	else if (_inputManager->keyPressed(SDLK_h)){
		_name += 'H';
		
	}
	else if (_inputManager->keyPressed(SDLK_i)){
		_name += 'I';
		
	}
	else if (_inputManager->keyPressed(SDLK_j)){
		_name += 'J';
		
	}
	else if (_inputManager->keyPressed(SDLK_k)){
		_name += 'K';
		
	}
	else if (_inputManager->keyPressed(SDLK_l)){
		_name += 'L';
		
	}
	else if (_inputManager->keyPressed(SDLK_m)){
		_name += 'M';
		
	}
	else if (_inputManager->keyPressed(SDLK_n)){
		_name += 'N';
		
	}
	else if (_inputManager->keyPressed(SDLK_o)){
		_name += 'O';
		
	}
	else if (_inputManager->keyPressed(SDLK_p)){
		_name += 'P';
		
	}
	else if (_inputManager->keyPressed(SDLK_q)){
		_name += 'Q';
		
	}
	else if (_inputManager->keyPressed(SDLK_r)){
		_name += 'R';
		
	}
	else if (_inputManager->keyPressed(SDLK_s)){
		_name += 'S';
		
	}
	else if (_inputManager->keyPressed(SDLK_t)){
		_name += 'T';
		
	}
	else if (_inputManager->keyPressed(SDLK_u)){
		_name += 'U';
		
	}
	else if (_inputManager->keyPressed(SDLK_v)){
		_name += 'V';
		
	}
	else if (_inputManager->keyPressed(SDLK_w)){
		_name += 'W';
		
	}
	else if (_inputManager->keyPressed(SDLK_x)){
		_name += 'X';
		
	}
	else if (_inputManager->keyPressed(SDLK_y)){
		_name += 'Y';
		
	}
	else if (_inputManager->keyPressed(SDLK_z)){
		_name += 'Z';
		
	}
	else if (_inputManager->keyPressed(SDLK_RETURN)){
		while (_name.size() < 3)
			_name += '!';
	}
	if (_name.size() > 2)
	{
		_done = true;
	}
}

int CharInput::size()
{
	return _name.size();
}

bool CharInput::done()
{
	return _done;
}

std::string CharInput::getName()
{
	return _name;
}

void CharInput::resetName()
{
	_name = "";
}