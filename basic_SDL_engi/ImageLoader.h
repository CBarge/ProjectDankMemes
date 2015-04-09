#pragma once
#include "GLTexture.h"

#include <string>

namespace basic_SDL_engi
{

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}