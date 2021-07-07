#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	void SetShaderProperties(const Shader& shaderProgram) const override;
};