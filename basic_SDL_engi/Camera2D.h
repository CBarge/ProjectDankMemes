#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:
	Camera2D();
	~Camera2D();

	void update();

	void setPosition(glm::vec2& newPosition){ position = newPosition; }
	void setScale(float newScale) { scale = newScale; }

	//getters
	glm::vec2 getPosition() { return position; }
	float getScale() { return scale; }
	glm::mat4 getCameraMatrix() { return cameraMatrix; }

private:
	float scale;
	glm::vec2 position;
	glm::mat4 cameraMatrix;
};

