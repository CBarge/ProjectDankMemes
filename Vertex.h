#pragma once

#include <GL/glew.h>

struct Position
{
	//this makes it so variable names can be referenced instead of array position
	float x;
	float y;
};

struct UV
{
	float u;
	float v;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

//for efficient storage of attributes!
struct Vertex
{
	Position position;
	
	UV uv;

	Color color;

	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
};