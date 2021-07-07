#include "SpotLight.h"

SpotLight::SpotLight() : Light()
{
	this->cutOffAngle = 20.0f;
	this->cutOffAngleOut = 25.0f;
}

SpotLight::~SpotLight()
{

}

void SpotLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("spotLight.position", this->transform.position);
	shaderProgram.SetVector3("spotLight.direction", -this->transform.GetUpVector());
	shaderProgram.SetFloat("spotLight.intensity", this->intensity);
	shaderProgram.SetFloat("spotLight.cutOffAngle", this->cutOffAngle);
	shaderProgram.SetFloat("spotLight.cutOffAngleOut", this->cutOffAngleOut);
}