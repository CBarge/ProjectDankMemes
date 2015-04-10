#include <SDL/SDL.h>

#include "Time.h"

namespace basic_SDL_engi
{
	FPSLimiter::FPSLimiter()
	{

	}

	void FPSLimiter::init(float max)
	{
		setFPS(max);
	}

	void FPSLimiter::setFPS(float max)
	{
		_max = max;
	}

	void FPSLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}
	//returns fps
	float  FPSLimiter::end()
	{
		calcFPS();
		float frameTicks = SDL_GetTicks() - _startTicks;
		if (1000.0f / _max > frameTicks)
		{
			SDL_Delay(1000.0f / _max - frameTicks);		//8ms delay, OS specific!
		}

		return _fps;
	}

	void FPSLimiter::calcFPS()
	{
		static const int SAMPLE_NUM = 10;
		static float frameTimes[SAMPLE_NUM];
		static int frame = 0;

		static float prevTicks = SDL_GetTicks();

		float currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - prevTicks;
		frameTimes[frame % SAMPLE_NUM] = _frameTime;

		prevTicks = currentTicks;

		int count;

		frame++;
		if (frame < SAMPLE_NUM)
		{
			count = frame;
		}
		else
		{
			count = SAMPLE_NUM;
		}

		float frameTimeAvg = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAvg += frameTimes[i];
		}
		frameTimeAvg /= count;

		if (frameTimeAvg > 0)
		{
			_fps = 1000.0f / frameTimeAvg;
		}
		else
		{
			_fps = 60.0f;
		}
	}

}