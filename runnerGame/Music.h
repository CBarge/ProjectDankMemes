#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include <vector>

class Music {
public:
	//inits all music 
	void initi();
	//plays music at index of _musics to be called in MainGame
	void playMusic(int index);
	//stops music
	void stopMusic(int index);
	void closePlayer();

	// close the interface once done
	void clearMusic();
private:
	//code to actually play the sound
	void play(Mix_Music* mMusic);

	static std::vector<Mix_Music*> _musics;
}