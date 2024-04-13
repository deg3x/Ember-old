#include "engine_pch.h"
#include "Camera.h"

#include "core/Renderer.h"
#include "core/Object.h"
#include "core/components/Transform.h"
#include "logger/Logger.h"

std::shared_ptr<Camera> Camera::ActiveCamera;

Camera::Camera()
{
    perspectiveFOV   = 60.0f;
    orthographicSize = 3.0f;
    nearClipping     = 0.01f;
    farClipping      = 1000.0f;
    projectionType   = CameraProjection::PERSPECTIVE;
}

glm::mat4x4 Camera::GetViewMatrix() const
{
    const glm::mat4x4 viewMatrix = glm::lookAt(owner->transform->GetPosition(), owner->transform->GetPosition() + owner->transform->GetForwardVector(), owner->transform->GetUpVector());

    return viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
    int viewportWidth;
    int viewportHeight;

    Renderer::GetViewportResolution(viewportWidth, viewportHeight);

    const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);

    switch(projectionType)
    {
    case CameraProjection::PERSPECTIVE:
        return glm::perspective(glm::radians(perspectiveFOV), aspectRatio, nearClipping, farClipping);
    case CameraProjection::ORTHOGRAPHIC:
        return glm::ortho(-aspectRatio * orthographicSize, aspectRatio * orthographicSize, -orthographicSize, orthographicSize, nearClipping, farClipping);
    default:
        Logger::Log(LogCategory::ERROR, "Unknown camera projection type", "Camera::GetProjectionMatrix");
        return glm::identity<glm::mat4x4>();
    }
}
