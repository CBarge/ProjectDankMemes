#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include <vector>

class SFX {
	public:
		//inits all sound effects 
		void init();
		//plays sound at index of _sounds to be called in MainGame
		static void playSound(int index);

		// close the interface once done
		static void clearFX();
	private:
		//code to actually play the sound
		static void play(Mix_Chunk* sound);

		static std::vector<Mix_Chunk*> _sounds;
};