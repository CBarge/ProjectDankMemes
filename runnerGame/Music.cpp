#include <stdio.h>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

// include class definition file
#include "Music.h"


void Music::initi()
{
	//initialize music
	_musics[0] = Mix_LoadMUS("MenuMusic.wav");
	_musics[1] = Mix_LoadMUS("GameMusic.wav");


	for (int i = 0; i < _musics.size(); i++)
	{
		if (_musics[i] == NULL)
		{
			//checks to make sure each one loaded correctly
			printf("Mixer error for %d: %s\n", i, Mix_GetError());
		}
	}
}

void Music::playMusic(int index)
{
	play(_musics[index]);
}

void Music::stopMusic(int index)
{
	//fades out music over 3 seconds
	Mix_FadeOutMusic(3000);
	Mix_FreeMusic(_musics[index]);
}

void Music::closePlayer()
{
	Mix_CloseAudio();
}
// clean up function for  music, to be called by the cleanup function of the main game
void Music::clearMusic() {


	//resets everything back to NULL
	for (int i = 0; i < _musics.size(); i++)
	{
		_musics[i] = NULL;
	}


}

void Music::play(Mix_Music* mMusic)
{
	//create mixer, defaults are freq of 22050 and 16bit
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	//check for error
	if (Mix_PlayingMusic() == -1)
	{
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
	}
	//plays sound on first open channel once
	else
	{
		Mix_FadeInMusic(mMusic, -1, 2000);
	}
	

}