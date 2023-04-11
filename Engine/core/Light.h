#pragma once
#include "Transform.h"
#include "Shader.h"
#include "Object.h"
#include "glm/glm.hpp"

class Light : public Object
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	float intensity;

protected:
	Light();
	virtual ~Light() = default;

public:
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};