#include "engine_pch.h"
#include "Light.h"
#include "core/Shader.h"
#include "core/Object.h"
#include "core/components/Transform.h"

#include "logger/Logger.h"

Light::Light()
{
	intensity        = 1.0f;
	diffuse          = glm::vec3(1.0f, 0.97f, 0.92f);
	ambient          = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLightData = {1.0f, 0.045f, 0.0075f};
	spotlightData  = { 12.5f, 13.0f, 1.0f, 0.045f, 0.0075f };
	lightType             = LightType::DIRECTIONAL;
}

void Light::SetShaderProperties(const Shader& shaderProgram) const
{
	shaderProgram.SetBool("directionalLight.isUsed", false);
	shaderProgram.SetBool("pointLight.isUsed", false);
	shaderProgram.SetBool("spotLight.isUsed", false);
	
	switch(lightType)
	{
	case LightType::DIRECTIONAL:
		shaderProgram.SetVector3("directionalLight.ambient", ambient);
		shaderProgram.SetVector3("directionalLight.diffuse", diffuse);
		shaderProgram.SetVector3("directionalLight.direction", -owner->transform->GetUpVector());
		shaderProgram.SetFloat("directionalLight.intensity", intensity);
		shaderProgram.SetBool("directionalLight.isUsed", true);
		break;
	case LightType::POINT:
		shaderProgram.SetVector3("pointLight.ambient", ambient);
		shaderProgram.SetVector3("pointLight.diffuse", diffuse);
		shaderProgram.SetVector3("pointLight.position", owner->transform->position);
		shaderProgram.SetFloat("pointLight.intensity", intensity);
		shaderProgram.SetFloat("pointLight.constantAttenuation", pointLightData.constantAttenuation);
		shaderProgram.SetFloat("pointLight.linearAttenuation", pointLightData.linearAttenuation);
		shaderProgram.SetFloat("pointLight.quadraticAttenuation", pointLightData.quadraticAttenuation);
		shaderProgram.SetBool("pointLight.isUsed", true);
		break;
	case LightType::SPOTLIGHT:
		shaderProgram.SetVector3("spotLight.ambient", ambient);
		shaderProgram.SetVector3("spotLight.diffuse", diffuse);
		shaderProgram.SetVector3("spotLight.position", owner->transform->position);
		shaderProgram.SetVector3("spotLight.direction", -owner->transform->GetUpVector());
		shaderProgram.SetFloat("spotLight.intensity", intensity);
		shaderProgram.SetFloat("spotLight.constantAttenuation", spotlightData.constantAttenuation);
		shaderProgram.SetFloat("spotLight.linearAttenuation", spotlightData.linearAttenuation);
		shaderProgram.SetFloat("spotLight.quadraticAttenuation", spotlightData.quadraticAttenuation);
		shaderProgram.SetFloat("spotLight.cutOffAngleCos", glm::cos(spotlightData.cutOffAngle));
		shaderProgram.SetFloat("spotLight.cutOffAngleOutCos", glm::cos(spotlightData.cutOffAngleOut));
		shaderProgram.SetBool("spotLight.isUsed", true);
		break;
	default:
		Logger::Log(LogCategory::ERROR, "Unknown light type submitted", "Light::SetShaderProperties");
		break;
	}
}
