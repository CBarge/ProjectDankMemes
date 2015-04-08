#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{

}


GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	_programID = glCreateProgram();

	//reference OpenGL shader comp on OGL wiki!
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		lethalError("Vertex shader could not be created...!");
	}
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		lethalError("Fragment shader could not be created...!");
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);

}

void GLSLProgram::linkShaders()
{
	//Taken from open source OGL wiki!
	//Comments are pretty explanitory, feel free to reference the wiki for more info

	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		//Use the infoLog as you see fit.
		std::printf("%s\n", &(errorLog[0]));
		lethalError("Shaders could not link!");

		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string& attribute)
//for things like position, color, texture, etc. Defined in vertex file
{
	glBindAttribLocation(_programID, _numAttributes++, attribute.c_str());
}

GLuint GLSLProgram::getUniformLocation(const std::string& uniformName)
{
	GLint locCheck = glGetUniformLocation(_programID, uniformName.c_str());
	if (locCheck == GL_INVALID_INDEX)
	{
		lethalError("Uniform " + uniformName + " not found in shader...!");
	}
	return locCheck;
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::compileShader(const std::string& filePath, GLuint id)
{
	//looking at the vertex shader parameter file
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail())
	{
		perror(filePath.c_str());
		lethalError("Failed to open " + filePath);
	}

	std::string fileContent = "";
	std::string line;

	while (std::getline(vertexFile, line))
	{
		fileContent += line + "\n";
	}

	vertexFile.close();

	const char* contentPtr = fileContent.c_str();
	glShaderSource(id, 1, &contentPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	//in the case of compilation failure
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		//maxLength includes NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);	//prevent leaks
		return;

		//printing error log
		std::printf("%s\n", &(errorLog[0]));

		//terminating with error message
		lethalError("Shader " + filePath + " could not compile!");
	}
}