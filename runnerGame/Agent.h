#pragma once

#include <glm/glm.hpp>
#include <basic_SDL_engi/SpriteBatch.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Player;
class Chaser;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	//virtual functions can be overidden by functions of the same name in a subclass
	virtual void update(const std::vector<std::string>& levelData, Player* player, std::vector<Chaser*>& chasers, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	virtual void draw(basic_SDL_engi::SpriteBatch& _spriteBatch);

	bool damage(float damage);
	//getter for _grounded
	bool feetOnGround();

	virtual void jump(float speed);

	glm::vec2 getPosition() const { return _position; }

protected:
	void checkTilePosition(const std::vector<std::string>& levelData,
							std::vector<glm::vec2>& collideTilePositions,
							char& tileType,
							float x, float y);

	void collideWithTile(glm::vec2 tilePos, char tileType);

	glm::vec2 _position;
	basic_SDL_engi::Color _color;
	float _speed;
	float _health;
	bool _grounded;
	float _yMomentum;
};

