#include "PointLight.h"

PointLight::PointLight() : Light()
{
	// values for point light of range ~100
	this->constantAttenuation = 1.0f;
	this->linearAttenuation = 0.045f;
	this->quadraticAttenuation = 0.0075f;
}

PointLight::~PointLight()
{

}

void PointLight::SetShaderProperties(const Shader& shaderProgram) const
{
	
}