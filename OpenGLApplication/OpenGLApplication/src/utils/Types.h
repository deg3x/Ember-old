#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};