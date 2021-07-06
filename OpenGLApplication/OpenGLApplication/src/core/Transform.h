#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

public:
	Transform();
	virtual ~Transform();

};