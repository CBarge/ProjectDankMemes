#include "Player.h"
#include <SDL/SDL.h>

Player::Player() : _frames(0)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, basic_SDL_engi::InputManager* inputManager, basic_SDL_engi::Camera2D* camera) {
	_frames = 0;
	_yMomentum = 0.0f;
	_speed = speed;
	_position = pos;
	_lastPos = _position;
	_inputManager = inputManager;
	_camera = camera;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;
	_health = 100;
	_grounded = true;
}

void Player::update(const std::vector<std::string>& levelData, Player* player, std::vector<Chaser*>& chasers, float deltaTime)
{
	if (_inputManager->keyPressed(SDLK_w)) {
		if (_grounded)
			jump(15.0f);
	}
	else if (_inputManager->keyDown(SDLK_s)) {
		_position.y -= (_speed / 2.0f) * deltaTime;
	}
	if (_inputManager->keyDown(SDLK_a)) {
		_position.x -= (_speed / 2.0f) * deltaTime;
	}
	else if (_inputManager->keyDown(SDLK_d)) {
		_position.x += _speed * deltaTime;
	}
	_position.x += _speed * deltaTime;
	_position.y += _yMomentum * deltaTime;

	if (!_grounded)
	{
		_yMomentum -= 0.5f;
	}

	if (_frames == 37) {
		_frames = 0;
	}
	else {
		_frames++;
	}

	collideWithLevel(levelData);
}

bool Player::dead()
{
	if (_health <= 0)
	{
		return true;
	}
	return false;
}

bool Player::win()
{
	return _won;
}

void Player::jump(float speed)
{
	_sfx = _audio.loadSFX("Sound_FX/jump.wav");
	_sfx.play(0);
	_yMomentum = speed;
	_grounded = false;
}

void Player::draw(basic_SDL_engi::SpriteBatch& _spriteBatch)
{
	static int stand = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_stand.png").id;
	static int walk1 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk01.png").id;
	static int walk2 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk02.png").id;
	static int walk3 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk03.png").id;
	static int walk4 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk04.png").id;
	static int walk5 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk05.png").id;
	static int walk6 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk06.png").id;
	static int walk7 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk07.png").id;
	static int walk8 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk08.png").id;
	static int walk9 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk09.png").id;
	static int walk10 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk10.png").id;
	static int walk11 = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_walk/PNG/p2_walk11.png").id;
	static int jumping = basic_SDL_engi::ResourceManager::getTexture("Textures/Player/p2_jump.png").id;
	int textureID = stand;
	if (_frames > 0 && _frames <= 4)
		textureID = walk1;
	else if (_frames > 4 && _frames <= 8)
		textureID = walk2;
	else if (_frames > 8 && _frames <= 12)
		textureID = walk3;
	else if (_frames > 12 && _frames <= 16)
		textureID = walk4;
	else if (_frames > 16 && _frames <= 20)
		textureID = walk5;
	else if (_frames > 20 && _frames <= 24)
		textureID = walk6;
	else if (_frames > 24 && _frames <= 28)
		textureID = walk7;
	else if (_frames > 28 && _frames <= 32)
		textureID = walk8;
	else if (_frames > 32 && _frames <= 36)
		textureID = walk9;
	else if (_frames > 36 && _frames <= 40)
		textureID = walk10;
	else if (_frames > 40 && _frames <= 44)
		textureID = walk11;

	if (!_grounded)
	{
		textureID = jumping;
	}

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

