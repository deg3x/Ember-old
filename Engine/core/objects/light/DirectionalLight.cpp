#include "DirectionalLight.h"

#include "../../Shader.h"
#include "../../components/Transform.h"

DirectionalLight::DirectionalLight() : Light()
{
	transform->rotation.x = 30.0f;
	transform->rotation.y = -30.0f;
}

void DirectionalLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("directionalLight.ambient", ambient);
	shaderProgram.SetVector3("directionalLight.diffuse", diffuse);
	shaderProgram.SetVector3("directionalLight.direction", -transform->GetUpVector());
	shaderProgram.SetFloat("directionalLight.intensity", intensity);
}