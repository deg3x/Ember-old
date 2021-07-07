#include "SpotLight.h"

SpotLight::SpotLight() : Light()
{
	this->cutOffAngle = 20.0f;
	this->cutOffAngleOuter = 25.0f;
}

SpotLight::~SpotLight()
{

}

void SpotLight::SetShaderProperties(const Shader& shaderProgram) const
{

}