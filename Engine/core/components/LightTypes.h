#pragma once

enum class LightType : uint8_t
{
    DIRECTIONAL = 0x00,
    POINT       = 0x01,
    SPOTLIGHT   = 0x02
};

struct DirectionalLightData
{
    Vector3 color;
    Vector3 ambient;
    Vector3 direction;
    float intensity;
};

struct PointLightData
{
    Vector3 color;
    Vector3 ambient;
    Vector3 position;
    float intensity;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct SpotLightData
{
    Vector3 color;
    Vector3 ambient;
    Vector3 position;
    Vector3 direction;
    float intensity;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float cutOffAngle;
    float cutOffAngleOut;
};