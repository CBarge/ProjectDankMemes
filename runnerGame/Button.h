#pragma once

#include <string>
#include <vector>

#include <basic_SDL_engi/InputManager.h>
#include <basic_SDL_engi/Camera2D.h>
#include <basic_SDL_engi/SpriteBatch.h>
#include <basic_SDL_engi/ResourceManager.h>

class Button
{
public:
	Button();
	~Button();

	void init(int buttonType, float x, float y, float width, float height, basic_SDL_engi::InputManager* inputManager, basic_SDL_engi::Camera2D* camera);

	bool checkPressed();

	void draw(basic_SDL_engi::SpriteBatch& _spriteBatch);

	basic_SDL_engi::InputManager* _inputManager;
	basic_SDL_engi::Camera2D* _camera;
private:
	glm::vec2 _position;
	basic_SDL_engi::Color _color;
	int _buttonType;
	float _width;
	float _height;
};

