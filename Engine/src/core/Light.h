#pragma once
#include "Transform.h"
#include "Shader.h"
#include "glm/glm.hpp"

class Light
{
public:
	Transform transform;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	float intensity;

protected:
	Light();
	virtual ~Light();

public:
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};