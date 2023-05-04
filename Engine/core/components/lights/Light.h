#pragma once

#include "glm/glm.hpp"
#include "../../components/Component.h"

class Shader;

class ENGINE_API Light : public Component
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	float intensity;

public:
	Light();
	virtual ~Light() = default;
	
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};