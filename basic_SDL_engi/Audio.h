#pragma once

#include <string>
#include <SDL/SDL_mixer.h>
#include <map>

namespace basic_SDL_engi
{
	class SFX
	{
	public:
		friend class Audio;

		//loops == 0, play one time
		void play(int loopNum = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music
	{
	public:
		friend class Audio;
		//loops == -1, play forever, loop == 1, play once
		void play(int loopNum = -1);

		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;
	};

	class Audio
	{
	public:
		Audio();
		~Audio();

		void init();
		void destroy();

		SFX loadSFX(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	private:
		std::map < std::string, Mix_Chunk*> m_fxMap;
		std::map < std::string, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};
}


