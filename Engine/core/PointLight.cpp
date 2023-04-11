#include "PointLight.h"

PointLight::PointLight() : Light()
{
	// values for point light of range ~100
	constantAttenuation = 1.0f;
	linearAttenuation = 0.045f;
	quadraticAttenuation = 0.0075f;
}

void PointLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("pointLight.ambient", ambient);
	shaderProgram.SetVector3("pointLight.diffuse", diffuse);
	shaderProgram.SetVector3("pointLight.position", transform.position);
	shaderProgram.SetFloat("pointLight.intensity", intensity);
	shaderProgram.SetFloat("pointLight.constantAttenuation", constantAttenuation);
	shaderProgram.SetFloat("pointLight.linearAttenuation", linearAttenuation);
	shaderProgram.SetFloat("pointLight.quadraticAttenuation", quadraticAttenuation);
}