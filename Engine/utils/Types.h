#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

struct VertexData
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	
	Color color;
};

struct WindowData
{
	int windowW;
	int windowH;
	float aspectRatio;
	Color clearColor = { 0.12f, 0.12f, 0.12f, 1.0f };
	unsigned int clearBits;
};

struct MouseData
{
	float sensitivity = 0.01f;
	bool leftButtonPressed;
	bool rightButtonPressed;
	bool middleButtonPressed;
	double lastMouseX;
	double lastMouseY;
	double leftMouseXOffset;
	double leftMouseYOffset;
	double rightMouseXOffset;
	double rightMouseYOffset;

	void ResetLeftMouseOffsetData()
	{
		leftMouseXOffset = 0.0;
		leftMouseYOffset = 0.0;
	}

	void ResetRightMouseOffsetData()
	{
		rightMouseXOffset = 0.0;
		rightMouseYOffset = 0.0;
	}
};

enum class MaterialType
{
	Unlit,
	Phong
};

struct MaterialProperties
{
	MaterialProperties() = default;
	virtual ~MaterialProperties() = default;
};

struct MaterialPropertiesUnlit : public MaterialProperties
{
	glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	MaterialPropertiesUnlit() : MaterialProperties() {}
};

struct MaterialPropertiesPhong : public MaterialProperties
{
	glm::vec4 diffuse       = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 specular      = glm::vec3(0.9f, 0.8f, 0.8f);
	float shininessExponent = 64.0f;

	MaterialPropertiesPhong() : MaterialProperties() {}
};