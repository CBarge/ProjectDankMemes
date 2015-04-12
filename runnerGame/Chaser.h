#pragma once

#include "Agent.h"

class Chaser : public Agent
{
public:
	Chaser();
	~Chaser();

	void init(float speed, glm::vec2 pos);

	void update(const std::vector<std::string>& levelData, Player* player, std::vector<Chaser*>& Chasers, float deltaTime) override;
};

