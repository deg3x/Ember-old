#pragma once

#include "Definitions.h"

class ENGINE_API Shader
{
private:
	unsigned int id;

public:
    Shader() = delete;
	Shader(const char* vertPath, const char* fragPath);

	void Use() const;

	unsigned int GetShaderID() const;
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVector3(const std::string& name, const glm::vec3& vector) const;
	void SetVector4(const std::string& name, const glm::vec4& vector) const;
	void SetMatrix4x4(const std::string& name, const glm::mat4x4& matrix) const;
    
private:
    int CheckShaderCompiled(unsigned int shaderID, const std::string& name);
    int CheckProgramLinked(unsigned int shaderProgramID, const std::string& name);
    int ReadCodeFromPath(const char* path, std::string& code);
};
