#pragma once

#include "glm/glm.hpp"
#include "../Object.h"

class Shader;

class Light : public Object
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	float intensity;

public:
	Light();
	virtual ~Light() override = default;
	
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};