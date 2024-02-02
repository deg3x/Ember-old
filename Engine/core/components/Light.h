#pragma once

#include "core/components/Component.h"

class Shader;

enum class LightType : uint8_t
{
	DIRECTIONAL = 0x00,
	POINT       = 0x01,
	SPOTLIGHT   = 0x02
};

struct PointLightData
{
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct SpotLightData
{
	float cutOffAngle;
	float cutOffAngleOut;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

class ENGINE_API Light : public Component
{
	MAKE_COMPONENT_TYPE(LIGHT)
	
public:
	glm::vec3 ambient;
	glm::vec3 color;
	float intensity;
	PointLightData pointLightData;
	SpotLightData spotLightData;
	LightType lightType;

public:
	Light();
	
	void SetShaderProperties(const Shader& shaderProgram, int lightIndex) const;
};