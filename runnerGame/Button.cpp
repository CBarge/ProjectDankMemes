#include "Button.h"
#include <SDL/SDL.h>
#include <iostream>

Button::Button()
{
}

Button::~Button()
{
}

void Button::init(int buttonType, float x, float y, float width, float height, basic_SDL_engi::InputManager* inputManager, basic_SDL_engi::Camera2D* camera)
{
	_position.x = x;
	_position.y = y;
	_width = width;
	_height = height;
	_camera = camera;
	_inputManager = inputManager;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;
	_buttonType = buttonType;
}

bool Button::checkPressed()
{
	if (_inputManager->keyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 cursorCoords = _inputManager->getCursorCoords();
		cursorCoords = _camera->convSCoordsToWCoords(cursorCoords);
		if ((cursorCoords.x >= _position.x && cursorCoords.x <= (_position.x + _width)) &&
			(cursorCoords.y >= _position.y && cursorCoords.y <= (_position.y + _height)))
		{
			std::cout << "Fuck\n";
			return true;
		}
	}
	return false;
}

void Button::draw(basic_SDL_engi::SpriteBatch& _spriteBatch)
{
	static int start = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/start.png").id;
	static int exit = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/Exit.png").id;
	static int music = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/musicbutt.png").id;
	static int settings = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/Settings.png").id;
	static int sound = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/sound.png").id;
	static int credits = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/Credits.png").id;
	static int leaderboard = basic_SDL_engi::ResourceManager::getTexture("Textures/buttons/unclicked/leaderboard.png").id;
	int textureID = start;
	switch (_buttonType)
	{
	case 0:
		textureID = start;
		break;
	case 1:
		textureID = exit;
		break;
	case 2:
		textureID = music;
		break;
	case 3:
		textureID = settings;
		break;
	case 4:
		textureID = sound;
		break;
	case 5:
		textureID = credits;
		break;
	case 6:
		textureID = leaderboard;
		break;
	}
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = _width;
	destRect.w = _height;
	

	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}