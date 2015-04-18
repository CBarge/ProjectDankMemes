#include "CharInput.h"

CharInput::CharInput(){
	charCount = 0;
}
CharInput::~CharInput(){

}

char CharInput::getChar(){
	if (_inputManager->keyPressed(SDLK_a)){
		return 'A';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_b)){
		return 'B';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_c)){
		return 'C';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_d)){
		return 'D';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_e)){
		return 'E';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_f)){
		return 'F';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_g)){
		return 'G';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_h)){
		return 'H';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_i)){
		return 'I';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_j)){
		return 'J';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_k)){
		return 'K';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_l)){
		return 'L';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_m)){
		return 'M';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_n)){
		return 'N';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_o)){
		return 'O';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_p)){
		return 'P';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_q)){
		return 'Q';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_r)){
		return 'R';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_s)){
		return 'S';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_t)){
		return 'T';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_u)){
		return 'U';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_v)){
		return 'V';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_w)){
		return 'W';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_x)){
		return 'X';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_y)){
		return 'Y';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_z)){
		return 'Z';
		charCount++;
	}
	else if (_inputManager->keyPressed(SDLK_RETURN)){
		return '1'; 
	}
}

