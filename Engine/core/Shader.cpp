#include "Shader.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
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

	id = glCreateProgram();
	glAttachShader(id, vertID);
	glAttachShader(id, fragID);
	glLinkProgram(id);

	CheckProgramLinked(id);

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

void Shader::Use() const
{
	glUseProgram(id);
}

unsigned int Shader::GetShaderID() const
{
	return id;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVector3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetVector4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetMatrix4x4(const std::string& name, const glm::mat4x4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
