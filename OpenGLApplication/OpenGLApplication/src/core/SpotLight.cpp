#include "SpotLight.h"
#include <glm/glm.hpp>

SpotLight::SpotLight() : Light()
{
	this->cutOffAngle = 12.5f;
	this->cutOffAngleOut = 13.0f;
	this->constantAttenuation = 1.0f;
	this->linearAttenuation = 0.045f;
	this->quadraticAttenuation = 0.0075f;
}

SpotLight::~SpotLight()
{

}

void SpotLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("spotLight.ambient", this->ambient);
	shaderProgram.SetVector3("spotLight.diffuse", this->diffuse);
	shaderProgram.SetVector3("spotLight.position", this->transform.position);
	shaderProgram.SetVector3("spotLight.direction", -this->transform.GetUpVector());
	shaderProgram.SetFloat("spotLight.intensity", this->intensity);
	shaderProgram.SetFloat("spotLight.constantAttenuation", this->constantAttenuation);
	shaderProgram.SetFloat("spotLight.linearAttenuation", this->linearAttenuation);
	shaderProgram.SetFloat("spotLight.quadraticAttenuation", this->quadraticAttenuation);
	shaderProgram.SetFloat("spotLight.cutOffAngleCos", glm::cos(this->cutOffAngle));
	shaderProgram.SetFloat("spotLight.cutOffAngleOutCos", glm::cos(this->cutOffAngleOut));
}