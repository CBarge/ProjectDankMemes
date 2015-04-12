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

		void update();

		void onPress(unsigned int keyID);
		void onRelease(unsigned int keyID);

		void setCursorCoords(float x, float y);

		//true if key is held down
		bool keyDown(unsigned int keyID);

		//true if key was pressed on current frame
		bool keyPressed(unsigned int keyID);

		//gets
		glm::vec2 getCursorCoords() { return _cursorCoords; }
	private:
		std::unordered_map <unsigned int, bool> _keyMap;
		std::unordered_map <unsigned int, bool> _prevKeyMap;
		glm::vec2 _cursorCoords;

		bool wasKeyDown(unsigned int keyID);
	};

}