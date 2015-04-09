#pragma once

#include <vector>

namespace basic_SDL_engi
{

	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}