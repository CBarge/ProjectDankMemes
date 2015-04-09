#pragma once
#include <GL/glew.h>
#include <string>

#include "GLTexture.h"
#include "ResourceManager.h"

namespace basic_SDL_engi
{

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID;	//vertex buffer objects!
		GLTexture _texture;

	};

}