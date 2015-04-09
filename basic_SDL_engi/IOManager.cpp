#include "IOManager.h"

#include <fstream>

namespace basic_SDL_engi
{

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		//reading in binary data!
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//getting file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//reduce file size by any header bytes present
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);	//gives buffer[0] gives c_str
		file.close();

		return true;
	}

}