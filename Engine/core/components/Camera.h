#pragma once

#include "Component.h"

enum class CameraProjection : uint8_t
{
    PERSPECTIVE  = 0x00,
    ORTHOGRAPHIC = 0x01
};

class ENGINE_API Camera : public Component
{
    MAKE_UNIQUE_COMPONENT()
    MAKE_COMPONENT_TYPE(CAMERA)

public:
    static std::shared_ptr<Camera> ActiveCamera;
    
    float perspectiveFOV;
    float orthographicSize;
    float nearClipping;
    float farClipping;
    CameraProjection projectionType;
    
public:
    Camera();

    Matrix4x4 GetViewMatrix() const;
    Matrix4x4 GetProjectionMatrix() const;
};
