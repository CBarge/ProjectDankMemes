#include "Level.h"

#include <basic_SDL_engi/Errors.h>
#include <fstream>
#include <iostream>
#include <basic_SDL_engi/ResourceManager.h>

Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	// Error checking
	if (file.fail()) {
		basic_SDL_engi::lethalError("Failed to open " + fileName);
	}

	std::string tmp;

	file >> tmp >> _numObstacles;

	std::getline(file, tmp); // Throw away the rest of the first line

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	basic_SDL_engi::Color whiteColor(255, 255, 255, 255);

	// Render all the tiles
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			// Grab the tile
			char tile = _levelData[y][x];

			// Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			// Process the tile
			switch (tile) {
			case 'G':
				_spriteBatch.draw(destRect, uvRect, basic_SDL_engi::ResourceManager::getTexture("Textures/Tiles/castle.png").id, 0.0f, whiteColor);
				break;
			case 'E':
				_spriteBatch.draw(destRect, uvRect, basic_SDL_engi::ResourceManager::getTexture("Textures/Tiles/castleCenter_rounded.png").id, 0.0f, whiteColor);
				break;
			case 'S':
				_spriteBatch.draw(destRect, uvRect, basic_SDL_engi::ResourceManager::getTexture("Textures/Items/spikes.png").id, 0.0f, whiteColor);
				break;
			case 'P':
				_spriteBatch.draw(destRect, uvRect, basic_SDL_engi::ResourceManager::getTexture("Textures/Items/springboardUp.png").id, 0.0f, whiteColor);
				break;
			case 'H':
				_spriteBatch.draw(destRect, uvRect, basic_SDL_engi::ResourceManager::getTexture("Textures/Tiles/castleCenter_rounded.png").id, 0.0f, whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'C':
				_levelData[y][x] = '.';
				_chaserPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}

	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}