#pragma once

enum class LightType : uint8_t
{
    DIRECTIONAL = 0x00,
    POINT       = 0x01,
    SPOTLIGHT   = 0x02
};

struct DirectionalLightData
{
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 direction;
    float intensity;
};

struct PointLightData
{
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 position;
    float intensity;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct SpotLightData
{
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 position;
    glm::vec3 direction;
    float intensity;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float cutOffAngle;
    float cutOffAngleOut;
};