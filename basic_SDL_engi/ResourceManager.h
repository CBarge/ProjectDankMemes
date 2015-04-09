#pragma once
#include "TextureCache.h"
#include <string>

namespace basic_SDL_engi
{

	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};

}