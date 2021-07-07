#pragma once
#include "Transform.h"
#include "Shader.h"

class Light
{
protected:
	Transform transform;
	float intensity;
	
	Light();
	virtual ~Light();

public:
	virtual void SetShaderProperties(const Shader& shaderProgram) const = 0;
};