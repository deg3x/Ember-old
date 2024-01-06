#include "Camera.h"

#include "core/Renderer.h"
#include "core/objects/Object.h"
#include "core/components/Transform.h"

glm::mat4x4 Camera::GetViewMatrix() const
{
	const glm::mat4x4 lookAtMatrix = glm::lookAt(parent->transform->position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	return lookAtMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
	int viewportWidth;
	int viewportHeight;

	Renderer::GetViewportResolution(viewportWidth, viewportHeight);

	const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
	
	return glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 1000.0f);
}
