#pragma once

#include "Object.h"
#include "glm/glm.hpp"
#include "../components/Transform.h"

class Camera : public Object
{
public:
	Camera();
	virtual ~Camera() = default;

	glm::mat4x4 GetViewMatrix() const;
};
