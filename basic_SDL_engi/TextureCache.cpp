#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>


namespace basic_SDL_engi
{

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//looks up the texture, sees if in the map
		std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);

		//if not in the map
		if (mit == _textureMap.end())
		{
			//loading texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);	//already error protected in imageloader

			//inserting into map
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Cached texture loaded...\n";
			return newTexture;
		}
		std::cout << "Uncached texture loaded...\n";
		//return texture in map
		return mit->second;
	}

}
