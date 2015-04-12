#include "Chaser.h"
#include "Player.h"

Chaser::Chaser()
{
}

Chaser::~Chaser()
{
}

void Chaser::init(float speed, glm::vec2 pos)
{
	_speed = speed;
	_position = pos;
	_health = 100;
	_color.r = 100;
	_color.g = 160;
	_color.b = 0;
	_color.a = 200;
}

void Chaser::update(const std::vector<std::string>& levelData, Player* player, std::vector<Chaser*>& Chasers, float deltaTime)
{
	glm::vec2 direction = glm::normalize(player->getPosition() - _position);
	_position += direction * _speed * deltaTime;

	//collideWithLevel(levelData);
}