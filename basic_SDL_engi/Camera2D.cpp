#include "Camera2D.h"

namespace basic_SDL_engi
{

	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMat(1.0f),
		_scale(1.0f),
		_needMatUpdate(true),
		_screenWidth(500),
		_screenHeight(500)
	{

	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMat = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::update()
	{
		if (_needMatUpdate)
		{
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);	//3rd param would be 3rd dimension
			_cameraMatrix = glm::translate(_orthoMat, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			_needMatUpdate = false;
		}
	}

	glm::vec2 Camera2D::convSCoordsToWCoords(glm::vec2 screenCoords)
	{
		screenCoords.y = _screenHeight - screenCoords.y;
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		screenCoords /= _scale;		//ingenuity! not mine.
		screenCoords += _position;
		return screenCoords;
	}
}