#include "SpotLight.h"

#include "glm/glm.hpp"
#include "../../Shader.h"
#include "../../components/Transform.h"

SpotLight::SpotLight() : Light()
{
	cutOffAngle = 12.5f;
	cutOffAngleOut = 13.0f;
	constantAttenuation = 1.0f;
	linearAttenuation = 0.045f;
	quadraticAttenuation = 0.0075f;
}

void SpotLight::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetVector3("spotLight.ambient", ambient);
	shaderProgram.SetVector3("spotLight.diffuse", diffuse);
	shaderProgram.SetVector3("spotLight.position", transform->position);
	shaderProgram.SetVector3("spotLight.direction", -transform->GetUpVector());
	shaderProgram.SetFloat("spotLight.intensity", intensity);
	shaderProgram.SetFloat("spotLight.constantAttenuation", constantAttenuation);
	shaderProgram.SetFloat("spotLight.linearAttenuation", linearAttenuation);
	shaderProgram.SetFloat("spotLight.quadraticAttenuation", quadraticAttenuation);
	shaderProgram.SetFloat("spotLight.cutOffAngleCos", glm::cos(cutOffAngle));
	shaderProgram.SetFloat("spotLight.cutOffAngleOutCos", glm::cos(cutOffAngleOut));
}
