#include "../deps/include/SDL/SDL.h"
#include "../deps/include/SDL/SDL_mixer.h"

#include <vector>

class SFX {
	public:
		// constructor method, which inits all sound effects 
		SFX();
		// destructor
		~SFX();

		void init()

		void playSound(int index);

		// functions to initialize the FX and close the interface once done
		void closeFX();
	private:
		void play(Mix_Chunk* sound);

		std::vector<Mix_Chunk*> _sounds;
};