#include "engine_pch.h"
#include "Light.h"
#include "core/Shader.h"
#include "core/Object.h"
#include "core/components/Transform.h"

#include "logger/Logger.h"

Light::Light()
{
	intensity        = 1.0f;
	color          = glm::vec3(1.0f, 0.97f, 0.92f);
	ambient          = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLightData = {1.0f, 0.045f, 0.0075f};
	spotLightData  = { 12.5f, 13.0f, 1.0f, 0.045f, 0.0075f };
	lightType        = LightType::DIRECTIONAL;
}

void Light::SetShaderProperties(const Shader& shaderProgram, int lightIndex) const
{
	std::string lightVar;
	
	switch(lightType)
	{
	case LightType::DIRECTIONAL:
		lightVar = "directionalLight[" + std::to_string(lightIndex) + "]";
		shaderProgram.SetVector3(lightVar + ".ambient", ambient);
		shaderProgram.SetVector3(lightVar + ".color", color);
		shaderProgram.SetVector3(lightVar + ".direction", -owner->transform->GetUpVector());
		shaderProgram.SetFloat(lightVar + ".intensity", intensity);
		break;
	case LightType::POINT:
		lightVar = "pointLight[" + std::to_string(lightIndex) + "]";
		shaderProgram.SetVector3(lightVar + ".ambient", ambient);
		shaderProgram.SetVector3(lightVar + ".color", color);
		shaderProgram.SetVector3(lightVar + ".position", owner->transform->position);
		shaderProgram.SetFloat(lightVar + ".intensity", intensity);
		shaderProgram.SetFloat(lightVar + ".constantAttenuation", pointLightData.constantAttenuation);
		shaderProgram.SetFloat(lightVar + ".linearAttenuation", pointLightData.linearAttenuation);
		shaderProgram.SetFloat(lightVar + ".quadraticAttenuation", pointLightData.quadraticAttenuation);
		break;
	case LightType::SPOTLIGHT:
		lightVar = "spotLight[" + std::to_string(lightIndex) + "]";
		shaderProgram.SetVector3(lightVar + ".ambient", ambient);
		shaderProgram.SetVector3(lightVar + ".color", color);
		shaderProgram.SetVector3(lightVar + ".position", owner->transform->position);
		shaderProgram.SetVector3(lightVar + ".direction", -owner->transform->GetUpVector());
		shaderProgram.SetFloat(lightVar + ".intensity", intensity);
		shaderProgram.SetFloat(lightVar + ".constantAttenuation", spotLightData.constantAttenuation);
		shaderProgram.SetFloat(lightVar + ".linearAttenuation", spotLightData.linearAttenuation);
		shaderProgram.SetFloat(lightVar + ".quadraticAttenuation", spotLightData.quadraticAttenuation);
		shaderProgram.SetFloat(lightVar + ".cutOffAngleCos", glm::cos(spotLightData.cutOffAngle));
		shaderProgram.SetFloat(lightVar + ".cutOffAngleOutCos", glm::cos(spotLightData.cutOffAngleOut));
		break;
	default:
		Logger::Log(LogCategory::ERROR, "Unknown light type submitted", "Light::SetShaderProperties");
		break;
	}
}
