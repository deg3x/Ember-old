#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	float cutOffAngle;
	float cutOffAngleOuter;

	SpotLight();
	~SpotLight();

	void SetShaderProperties(const Shader& shaderProgram) const override;
};