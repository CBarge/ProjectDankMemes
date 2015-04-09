#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

namespace basic_SDL_engi
{

	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		//DEEEEEEEEEEEEEESTRUCTION!!!
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);		//error protected

		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		Vertex vertexData[6];

		//making a quad
		//first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle. if you want to make a loop feel free it won't change much
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		//temporary color set
		for (int i = 0; i < 6; i++)
		{
			vertexData[4].setColor(255, 0, 255, 255);
		}

		vertexData[4].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);

		//bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//unbind
		glBindBuffer(GL_BUFFER_ACCESS, 0);
	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		//bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		//sending array of position

		//draw vertices to screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//removing attrib
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		//unbind
		glBindBuffer(GL_BUFFER_ACCESS, 0);
	}

}