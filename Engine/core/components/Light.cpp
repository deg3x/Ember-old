#include "engine_pch.h"
#include "Light.h"
#include "core/Shader.h"
#include "core/Object.h"
#include "core/components/Transform.h"

#include "logger/Logger.h"

Light::Light()
{
    constexpr glm::vec3 direction = { 0.0f, -1.0f, 0.0f };
    constexpr glm::vec3 position  = { 0.0f, 0.0f, 0.0f };
    constexpr glm::vec3 color     = { 1.0f, 0.97f, 0.92f };
    constexpr glm::vec3 ambient   = { 0.05f, 0.05f, 0.05f };
    constexpr float intensity     = 1.0f;
    constexpr float constAtten    = 1.0f;
    constexpr float linearAtten   = 0.045f;
    constexpr float quadrAtten    = 0.0075f;
    constexpr float cutOff        = 12.5f;
    constexpr float cutOffOut     = 13.0f;
    
    directional = { color, ambient, direction, intensity };
    point       = { color, ambient, position, intensity, constAtten, linearAtten, quadrAtten};
    spot        = { color, ambient, position, direction, intensity, constAtten, linearAtten, quadrAtten, cutOff, cutOffOut };

    type = LightType::DIRECTIONAL;
}

void Light::Tick()
{
    Component::Tick();

    const glm::vec3 direction = -owner->transform->GetUpVector();
    const glm::vec3 position  = owner->transform->GetPosition();
    
    directional.direction = direction;
    point.position        = position;
    spot.position         = position;
    spot.direction        = direction;
}

void Light::SetShaderProperties(const Shader& shaderProgram, int lightIndex) const
{
    std::string lightVar;
    
    switch(type)
    {
    case LightType::DIRECTIONAL:
        lightVar = "dirLights[" + std::to_string(lightIndex) + "]";
        shaderProgram.SetVector3(lightVar + ".ambient", directional.ambient);
        shaderProgram.SetVector3(lightVar + ".color", directional.color);
        shaderProgram.SetVector3(lightVar + ".direction", directional.direction);
        shaderProgram.SetFloat(lightVar + ".intensity", directional.intensity);
        break;
    case LightType::POINT:
        lightVar = "pointLights[" + std::to_string(lightIndex) + "]";
        shaderProgram.SetVector3(lightVar + ".ambient", point.ambient);
        shaderProgram.SetVector3(lightVar + ".color", point.color);
        shaderProgram.SetVector3(lightVar + ".position", point.position);
        shaderProgram.SetFloat(lightVar + ".intensity", point.intensity);
        shaderProgram.SetFloat(lightVar + ".constantAttenuation", point.constantAttenuation);
        shaderProgram.SetFloat(lightVar + ".linearAttenuation", point.linearAttenuation);
        shaderProgram.SetFloat(lightVar + ".quadraticAttenuation", point.quadraticAttenuation);
        break;
    case LightType::SPOTLIGHT:
        lightVar = "spotLights[" + std::to_string(lightIndex) + "]";
        shaderProgram.SetVector3(lightVar + ".ambient", spot.ambient);
        shaderProgram.SetVector3(lightVar + ".color", spot.color);
        shaderProgram.SetVector3(lightVar + ".position", spot.position);
        shaderProgram.SetVector3(lightVar + ".direction", spot.direction);
        shaderProgram.SetFloat(lightVar + ".intensity", spot.intensity);
        shaderProgram.SetFloat(lightVar + ".constantAttenuation", spot.constantAttenuation);
        shaderProgram.SetFloat(lightVar + ".linearAttenuation", spot.linearAttenuation);
        shaderProgram.SetFloat(lightVar + ".quadraticAttenuation", spot.quadraticAttenuation);
        shaderProgram.SetFloat(lightVar + ".cutOffAngleCos", glm::cos(spot.cutOffAngle));
        shaderProgram.SetFloat(lightVar + ".cutOffAngleOutCos", glm::cos(spot.cutOffAngleOut));
        break;
    default:
        Logger::Log(LogCategory::ERROR, "Unknown light type submitted", "Light::SetShaderProperties");
        break;
    }
}
