#include "SpriteBatch.h"

#include <algorithm>

namespace basic_SDL_engi
{

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */)
	{
		_sortType = sortType;
		_renderBatches.clear();
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& rectDest, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(rectDest.x, rectDest.y + rectDest.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->botLeft.color = color;
		newGlyph->botLeft.setPosition(rectDest.x, rectDest.y);
		newGlyph->botLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->botRight.color = color;
		newGlyph->botRight.setPosition(rectDest.x + rectDest.z, rectDest.y);
		newGlyph->botRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(rectDest.x + rectDest.z, rectDest.y + rectDest.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);

		if (_glyphs.empty())
		{
			return;
		}

		int offset = 0;
		int currVertex = 0;
		_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
		vertices[currVertex++] = _glyphs[0]->topLeft;
		vertices[currVertex++] = _glyphs[0]->botLeft;
		vertices[currVertex++] = _glyphs[0]->botRight;
		vertices[currVertex++] = _glyphs[0]->botRight;
		vertices[currVertex++] = _glyphs[0]->topRight;
		vertices[currVertex++] = _glyphs[0]->topLeft;
		offset += 6;

		for (int currGlyph = 1; currGlyph < _glyphs.size(); currGlyph++)
		{
			if (_glyphs[currGlyph]->texture != _glyphs[currGlyph - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphs[currGlyph]->texture);
			}
			else
			{
				_renderBatches.back().numVertices += 6;		//add more vertices
			}
			vertices[currVertex++] = _glyphs[currGlyph]->topLeft;
			vertices[currVertex++] = _glyphs[currGlyph]->botLeft;
			vertices[currVertex++] = _glyphs[currGlyph]->botRight;
			vertices[currVertex++] = _glyphs[currGlyph]->botRight;
			vertices[currVertex++] = _glyphs[currGlyph]->topRight;
			vertices[currVertex++] = _glyphs[currGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orhpaning that stupid buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//ul data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		//to prevent memory leaks
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//binds
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//unbind
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;

		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return(a->depth < b->depth);	//hooray for logic!
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return(a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return(a->texture < b->texture);
	}

}

