#pragma once
#include "glm/glm.hpp"
#include "Transform.h"

class Camera
{
public:
	Transform transform;

	Camera();
	virtual ~Camera();

	glm::mat4x4 GetViewMatrix() const;
};
