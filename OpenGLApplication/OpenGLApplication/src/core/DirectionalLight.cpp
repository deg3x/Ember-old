#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("directionalLight.ambient", ambient);
	shaderProgram.SetVector3("directionalLight.diffuse", diffuse);
	shaderProgram.SetVector3("directionalLight.direction", -transform.GetUpVector());
	shaderProgram.SetFloat("directionalLight.intensity", intensity);
}