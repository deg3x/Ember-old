#pragma once
#include "Transform.h"
#include "Shader.h"
#include "glm/glm.hpp"

class Light
{
protected:
	Transform transform;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	float intensity;
	
	Light();
	virtual ~Light();

public:
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};