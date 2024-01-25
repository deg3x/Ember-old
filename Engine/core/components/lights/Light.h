#pragma once

#include "core/components/Component.h"

class Shader;

class ENGINE_API Light : public Component
{
	MAKE_COMPONENT_TYPE(LIGHT)
	
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	float intensity;

public:
	Light();
	virtual ~Light() = default;
	
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};