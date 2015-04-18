#include "Audio.h"
#include "Errors.h"

namespace basic_SDL_engi
{
	void SFX::play(int loopNum)
	{
		if (Mix_PlayChannel(-1, m_chunk, loopNum) == -1)
		{
			if (Mix_PlayChannel(0, m_chunk, loopNum) == -1) {
				lethalError("Mix_PlayChannel error..." + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loopNum)
	{
		Mix_PlayMusic(m_music, loopNum);
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	Audio::Audio()
	{
		//nothin'.
	}

	Audio::~Audio()
	{
		destroy();
	}

	void Audio::init()
	{
		//basic initialization,
		//parameter is comb of file types, default is WAV
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			lethalError("Mix_Init Error..." + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			lethalError("Mix_OpenAudio Error..." + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}

	void Audio::destroy()
	{
		if (m_isInitialized)
		{
			m_isInitialized = false;
			Mix_Quit();
		}
	}

	SFX Audio::loadSFX(const std::string& filePath)
	{
		//finding audio in cache
		auto it = m_fxMap.find(filePath);

		SFX effect;

		if (it == m_fxMap.end())
		{
			//not found, needs to load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				lethalError("Mix_LoadWAV Error..." + std::string(Mix_GetError()));
			}

			effect.m_chunk = chunk;
			m_fxMap[filePath] = chunk;
		}
		else
		{
			//already there
			effect.m_chunk = it->second;
		}
	}

	Music Audio::loadMusic(const std::string& filePath)
	{
		//finding audio in cache
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end())
		{
			//not found, needs to load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				lethalError("Mix_LoadMUS Error..." + std::string(Mix_GetError()));
			}

			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic;
		}
		else
		{
			//already there
			music.m_music = it->second;
		}
	}
}