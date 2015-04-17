#include "../deps/include/SDL/SDL.h"
#include "../deps/include/SDL/SDL_mixer.h"

#include <vector>

class SFX {
	public:
		//inits all sound effects 
		void init();
		//plays sound at index of _sounds to be called in MainGame
		void playSound(int index);

		// close the interface once done
		void clearFX();
	private:
		//code to actually play the sound
		void play(Mix_Chunk* sound);

		std::vector<Mix_Chunk*> _sounds;
};