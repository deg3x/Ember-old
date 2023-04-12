#include "Camera.h"

Camera::Camera() : Object()
{
	transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
	transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);
}

glm::mat4x4 Camera::GetViewMatrix() const
{
	return glm::lookAt(transform->position, transform->position + transform->GetForwardVector(), transform->GetUpVector());
}