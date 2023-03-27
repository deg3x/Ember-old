#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	// Make private later and create range to attenuation resolution
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	PointLight();
	~PointLight();

	void SetShaderProperties(const Shader& shaderProgram) const override;
};