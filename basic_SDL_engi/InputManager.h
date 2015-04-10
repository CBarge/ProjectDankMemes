#pragma once
#include <glm/glm.hpp>

#include <unordered_map>

namespace basic_SDL_engi
{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void onPress(unsigned int keyID);
		void onRelease(unsigned int keyID);

		void setCursorCoords(float x, float y);

		bool keyPressed(unsigned int keyID);

		//gets
		glm::vec2 getCursorCoords() { return _cursorCoords; }
	private:
		std::unordered_map <unsigned int, bool> _keyMap;
		glm::vec2 _cursorCoords;
	};

}