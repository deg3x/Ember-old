#pragma once
#include "Transform.h"

class Camera
{
public:
	Transform transform;

	Camera();
	virtual ~Camera();
};