#pragma once

#include "Agent.h"

#include <basic_SDL_engi/InputManager.h>
#include <basic_SDL_engi/Camera2D.h>
#include <basic_SDL_engi/SpriteBatch.h>
#include <basic_SDL_engi/ResourceManager.h>

class Player : public Agent
{
public:
	Player();
	~Player();
	
	void init(float speed, glm::vec2 pos, basic_SDL_engi::InputManager* inputManager, basic_SDL_engi::Camera2D* camera);

	bool dead();

	bool win();

	void jump(float speed) override;

	void update(const std::vector<std::string>& levelData, Player* player, std::vector<Chaser*>& Chasers, float deltaTime) override;

	void draw(basic_SDL_engi::SpriteBatch& _spriteBatch) override;

private:
	basic_SDL_engi::InputManager* _inputManager;
	basic_SDL_engi::Camera2D* _camera;

	glm::vec2 _direction;
	int _frames;
};

