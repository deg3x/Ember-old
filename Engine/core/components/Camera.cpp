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

Matrix4x4 Camera::GetViewMatrix() const
{
    const Vector3 eye = owner->transform->GetPosition();
    const Vector3 dir = owner->transform->GetForwardVector();
    const Vector3 up  = owner->transform->GetUpVector();
    
    const Matrix4x4 viewMatrix = Matrix4x4::LookAt(eye, eye + dir, up);

    return viewMatrix;
}

Matrix4x4 Camera::GetProjectionMatrix() const
{
    int viewportWidth;
    int viewportHeight;

    Renderer::GetViewportResolution(viewportWidth, viewportHeight);

    const float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);

    switch(projectionType)
    {
    case CameraProjection::PERSPECTIVE:
        return Matrix4x4::Perspective(perspectiveFOV * DEG2RAD, aspectRatio, nearClipping, farClipping);
        //return glm::perspective(glm::radians(perspectiveFOV), aspectRatio, nearClipping, farClipping);
    case CameraProjection::ORTHOGRAPHIC:
        return Matrix4x4::Orthographic(
            -aspectRatio * orthographicSize,
            aspectRatio * orthographicSize,
            -orthographicSize,
            orthographicSize,
            nearClipping,
            farClipping
        );
        //return glm::ortho(-aspectRatio * orthographicSize, aspectRatio * orthographicSize, -orthographicSize, orthographicSize, nearClipping, farClipping);
    default:
        Logger::Log(LogCategory::ERROR, "Unknown camera projection type", "Camera::GetProjectionMatrix");
        return Matrix4x4();
    }
}
