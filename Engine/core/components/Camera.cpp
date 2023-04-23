#include "Camera.h"

#include "Transform.h"
#include "../objects/Object.h"

glm::mat4x4 Camera::GetViewMatrix() const
{
	return glm::lookAt(parent->transform->position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//return glm::lookAt(parent->transform->position, parent->transform->position + parent->transform->GetForwardVector(), parent->transform->GetUpVector());
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);
}
