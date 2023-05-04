#pragma once

#include "Light.h"

class ENGINE_API DirectionalLight : public Light
{
public:
	DirectionalLight() = default;
	virtual ~DirectionalLight() override = default;

	void SetShaderProperties(const Shader& shaderProgram) const override;
};