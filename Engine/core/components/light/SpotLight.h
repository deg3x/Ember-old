#pragma once

#include "Light.h"

class SpotLight : public Light
{
public:
	float cutOffAngle;
	float cutOffAngleOut;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

public:
	SpotLight();
	virtual ~SpotLight() override = default;

	void SetShaderProperties(const Shader& shaderProgram) const override;
};