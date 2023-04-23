#include "Camera.h"

#include "Transform.h"
#include "../objects/Object.h"

glm::mat4x4 Camera::GetViewMatrix() const
{
	return glm::lookAt(parent->transform->position, parent->transform->position + parent->transform->GetForwardVector(), parent->transform->GetUpVector());
}