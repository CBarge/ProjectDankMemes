#pragma once

#include <string>
#include <vector>

#include <basic_SDL_engi/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string& fileName);
	~Level();

	void draw();

	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	int getNumObstacles() const { return _numObstacles; }

	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& getChaserPositions() const { return _chaserPositions; }

private:
	int _numObstacles;
	std::vector<std::string> _levelData;
	basic_SDL_engi::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _chaserPositions;
};

