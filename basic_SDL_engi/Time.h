#pragma once

namespace basic_SDL_engi
{

	class FPSLimiter
	{
	public:
		FPSLimiter();

		void init(float max);
		void setFPS(float max);
		void begin();
		//returns fps
		float end();
	private:
		void calcFPS();

		float _max;
		float _fps;
		float _frameTime;
		unsigned int _startTicks;
	};

}