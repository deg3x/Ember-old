#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
private:
	unsigned int id;

	int CheckShaderCompiled(GLuint shaderID);
	int CheckProgramLinked(GLuint shaderProgramID);
	int ReadCodeFromPath(const char* path, std::string& code);

public:
	Shader(const char* vertPath, const char* fragPath);

	void Use();

	unsigned int GetShaderID();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetUniform3f(const std::string& name, glm::vec3 value) const;
	void SetUniform4f(const std::string& name, glm::vec4 value) const;
};