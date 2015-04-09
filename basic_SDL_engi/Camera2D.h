#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace basic_SDL_engi
{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		//this creates orthomat and gets screen info
		void init(int screenWidth, int screenHeight);

		//updates camera matrix...if neccessary
		void update();

		//sets
		void setPosition(glm::vec2& newPosition){ _position = newPosition; _needMatUpdate = true; }
		void setScale(float newScale) { _scale = newScale; _needMatUpdate = true; }

		//gets
		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth;
		int _screenHeight;
		bool _needMatUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMat;
	};

}

