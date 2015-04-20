#include "Agent.h"
#include <basic_SDL_engi/ResourceManager.h>
#include "Level.h"
#include <algorithm>

Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData)
{
	//By this method, all hitboxes are literally boxes. To wrap the hitbox around the 
	//sprite, once the program realizes the bounding boxes are touching, it would then check
	//if the pixels at the overlap location actually contain non-alpha particles
	//painstaking

	//vector containing collisions, initialized with no elements
	std::vector<glm::vec2> collideTilePositions;

	char tileType = '.';

	//has to check every corner, adds to vector if there is overlap
	//1
	checkTilePosition(levelData, collideTilePositions, tileType, _position.x, _position.y);
	//2
	checkTilePosition(levelData, collideTilePositions, tileType, _position.x + AGENT_WIDTH, _position.y);
	//3
	checkTilePosition(levelData, collideTilePositions, tileType, _position.x, _position.y + AGENT_WIDTH);
	//4
	checkTilePosition(levelData, collideTilePositions, tileType, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

	//check if collision happened by seeing if vector contains elements
	if (collideTilePositions.size() == 0)
	{
		_grounded = false;
		return false;
	}

	//actual collision
	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i], tileType);
	}
	return true;
}

bool Agent::collideWithAgent(Agent* agent)
{
	// Minimum distance before there is a collision
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	// Center position of this agent
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	// Center position of the parameter agent
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	// Distance vector between the two agents
	glm::vec2 distVec = centerPosA - centerPosB;

	// Length of the distance vector
	float distance = glm::length(distVec);

	// Depth of the collision
	float collisionDepth = MIN_DISTANCE - distance;

	// If collision depth > 0 then we did collide
	if (collisionDepth > 0) {

		// Get the direction times the collision depth so we can push them away from each other
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		// Push them in opposite directions
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;

		return true;
	}
	return false;
}

void Agent::draw(basic_SDL_engi::SpriteBatch& _spriteBatch)
{
	static int textureID = basic_SDL_engi::ResourceManager::getTexture("Textures/Enemies/blockerMad.png").id;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

bool Agent::damage(float damage)
{
	_health -= damage;
	// If we died, return true
	if (_health <= 0) {
		return true;
	}
	return false;
}

bool Agent::feetOnGround()
{
	return _grounded;
}

void Agent::victory()
{
	_won = true;
}

void Agent::jump(float speed)
{
	return;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, char& tileType, float x, float y)
{
	// Get the position of this corner in grid-space
	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	// If we are outside the world, just return
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return;
	}

	//otherwise if not air collide normally
	if (levelData[gridPos.y][gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
		tileType = levelData[gridPos.y][gridPos.x];
	}
}

// AABB (Axis Aligned Bounding Box) collision
void Agent::collideWithTile(glm::vec2 tilePos, char tileType)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	// Center position of the agent
	glm::vec2 centerAgentPos = _position + glm::vec2(AGENT_RADIUS);
	// Vector from the agent to the tile
	glm::vec2 distVec = centerAgentPos - tilePos;

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	// If either the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0)
	{
		switch (tileType)
		{
		case 'S':
			//case where tile is a spike
			damage(1.0f);
			jump(10.0f);
			_grounded = false;
			break;
		case 'P':
			jump(20.0f);
			_grounded = false;
			break;
		case 'H':
			_position.y += TILE_WIDTH;
			break;
		case 'T':
		case 'D':
			victory();
			break;
		default:
			//normal tile
			// Check which collision depth is less
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
			{
				// X collsion depth is smaller so we push in X direction
				if (distVec.x < 0)
				{
					_position.x -= xDepth + 2.0f;
				}
				else
				{
					//_position.x += xDepth;
				}
			}
			else
			{
				// Y collsion depth is smaller so we push in y direction
				if (distVec.y < 0)
				{
					_position.y -= yDepth;
				}
				else
				{
					_position.y += yDepth;
					_grounded = true;
				}
			}
			break;
		}
	}
	
}