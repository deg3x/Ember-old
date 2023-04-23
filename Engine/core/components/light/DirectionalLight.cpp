#include "DirectionalLight.h"

#include "../../Shader.h"
#include "../../objects/Object.h"
#include "../../components/Transform.h"

void DirectionalLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("directionalLight.ambient", ambient);
	shaderProgram.SetVector3("directionalLight.diffuse", diffuse);
	shaderProgram.SetVector3("directionalLight.direction", -parent->transform->GetUpVector());
	shaderProgram.SetFloat("directionalLight.intensity", intensity);
}