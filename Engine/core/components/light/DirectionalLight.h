#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;
	virtual ~DirectionalLight() override = default;

	void SetShaderProperties(const Shader& shaderProgram) const override;
};