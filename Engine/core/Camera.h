#pragma once
#include "glm/glm.hpp"
#include "Transform.h"
#include "Object.h"

class Camera : public Object
{
public:
	Camera();
	virtual ~Camera() = default;

	glm::mat4x4 GetViewMatrix() const;
};
