#include "engine_pch.h"
#include "DirectionalLight.h"

#include "core/Shader.h"
#include "core/objects/Object.h"
#include "core/components/Transform.h"

void DirectionalLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("directionalLight.ambient", ambient);
	shaderProgram.SetVector3("directionalLight.diffuse", diffuse);
	shaderProgram.SetVector3("directionalLight.direction", -owner->transform->GetUpVector());
	shaderProgram.SetFloat("directionalLight.intensity", intensity);
}