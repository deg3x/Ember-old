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
	shaderProgram.SetVector3("pointLight.ambient", this->ambient);
	shaderProgram.SetVector3("pointLight.diffuse", this->diffuse);
	shaderProgram.SetVector3("pointLight.position", this->transform.position);
	shaderProgram.SetFloat("pointLight.intensity", this->intensity);
	shaderProgram.SetFloat("pointLight.constantAttenuation", this->constantAttenuation);
	shaderProgram.SetFloat("pointLight.linearAttenuation", this->linearAttenuation);
	shaderProgram.SetFloat("pointLight.quadraticAttenuation", this->quadraticAttenuation);
}