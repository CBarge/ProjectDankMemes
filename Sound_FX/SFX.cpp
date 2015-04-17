// include relevant SDL dependecies and C dependencies
#include "../deps/include/SDL/SDL.h"
#include "../deps/include/SDL/SDL_mixer.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <SDL/SDL_mixer.h>

// include class definition file
#include "SFX.h"


void SFX::init()
{
	//keep doing this for all of them
	//applause for win boo for a loss
	_sounds[0] = Mix_LoadWAV("applause.wav");
	_sounds[1] = Mix_LoadWAV("booing.wav");
	//obstacle sfx
	_sounds[2] = Mix_LoadWAV("m_arc.wav");
	_sounds[3] = Mix_LoadWAV("m_bubble");
	_sounds[4] = Mix_LoadWAV("m_earcing.wav"); //check this one
	//sfx for menu navigation
	_sounds[5] = Mix_LoadWAV("m_change_setting.wav"); //remove if settings aren't finished
	_sounds[6] = Mix_LoadWAV("m_menu_navigate.wav");
	//player sfx
	_sounds[7] = Mix_LoadWAV("m_heartbeat.wav");
	_sounds[8] = Mix_LoadWAV("m_jump.wav");
	_sounds[9] = Mix_LoadWAV("m_death_scream.wav"); //check this one

	
	for (int i = 0; i < _sounds.size(); i++)
	{
		if (_sounds[i] == NULL)
		{
			//checks to make sure each one loaded correctly
			printf("Mixer error for "+i+" : %s\n", Mix_GetError());
			
		}
	}
}

void SFX::playSound(int index)
{
	play(_sounds[index]);
}

// clean up function for sound effects, to be called by the cleanup function of the main game
void SFX::clearFX() {
    

	//resets everything back to NULL
	for (int i = 0; i < _sounds.size(); i++)
	{
		_sounds[i] = NULL;
	}

    
}

void play(Mix_Chunk* sound)
{
	//create mixer, defaults are freq of 22050 and 16bit
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	//check for error
	if (Mix_PlayChannel(-1, sample, 0) == -1)
	{
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
	}
	//plays sound on first open channel once
	else
	{
		Mix_PlayChannel(-1, sound, 0);
	}
	//free chunk of the sfx that was loaded
	Mix_FreeChunk(sound);
	//closes mixer
	Mix_CloseAudio();
	
}