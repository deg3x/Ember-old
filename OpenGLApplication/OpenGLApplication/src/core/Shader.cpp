#include "shader.h"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertPath, const char* fragPath)
{
	std::string vCode;
	std::string fCode;

	ReadCodeFromPath(vertPath, vCode);
	ReadCodeFromPath(fragPath, fCode);

	const char* vertCode = vCode.c_str();
	const char* fragCode = fCode.c_str();

	unsigned int vertID;
	unsigned int fragID;

	vertID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertID, 1, &vertCode, NULL);
	glCompileShader(vertID);

	CheckShaderCompiled(vertID);

	fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragID, 1, &fragCode, NULL);
	glCompileShader(fragID);

	CheckShaderCompiled(fragID);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertID);
	glAttachShader(this->ID, fragID);
	glLinkProgram(this->ID);

	CheckProgramLinked(this->ID);

	glDeleteShader(vertID);
	glDeleteShader(fragID);
}

int Shader::ReadCodeFromPath(const char* path, std::string& code)
{
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);

		std::stringstream vertStream;
		std::stringstream fragStream;

		vertStream << file.rdbuf();

		file.close();

		code = vertStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_COULD_NOT_BE_READ::" << path << std::endl;
		return -1;
	}

	return 0;
}

int Shader::CheckShaderCompiled(GLuint shaderID)
{
	int success;
	char info[512];

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, info);
		std::cerr << "ERROR :: SHADER :: VERTEX :: COMPILATION_FAILED\n" << info << std::endl;
	}

	return success;
}

int Shader::CheckProgramLinked(GLuint shaderProgramID)
{
	int success;
	char info[512];

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, info);
		std::cerr << "ERROR :: SHADER :: PROGRAM :: LINKING_FAILED\n" << info << std::endl;
	}

	return success;
}

void Shader::Use()
{
	glUseProgram(this->ID);
}

unsigned int Shader::GetShaderID()
{
	return this->ID;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}