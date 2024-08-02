#pragma once

#include "core/components/Component.h"

enum class CoordSpace
{
    LOCAL,
    WORLD
};

class ENGINE_API Transform : public Component
{
    MAKE_UNIQUE_COMPONENT()
    MAKE_COMPONENT_TYPE(TRANSFORM)

public:
    static const Vector3 WorldForward;
    static const Vector3 WorldRight;
    static const Vector3 WorldUp;
    
private:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    Vector3 pivot;

private:
    bool isModelUpdated = false;
    Matrix4x4 localModelMatrix = Matrix4x4();
    
public:
    Transform();
    Transform(const Vector3& initPosition, const Quaternion& initRotation, const Vector3& initScale);
    Transform(const Vector3& initPosition, const Quaternion& initRotation, const Vector3& initScale, const Vector3& initPivotOffset);
    virtual ~Transform() = default;

    void Tick() override;

    void Translate(const Vector3& translateValue);
    void Rotate(const Quaternion& rotateValue, CoordSpace space = CoordSpace::LOCAL);
    void Scale(const Vector3& scaleValue);

    Vector3 GetPosition(CoordSpace space = CoordSpace::LOCAL) const;
    Quaternion GetRotation(CoordSpace space = CoordSpace::LOCAL) const;
    Vector3 GetRotationEuler(CoordSpace space = CoordSpace::LOCAL) const;
    Vector3 GetScale(CoordSpace space = CoordSpace::LOCAL) const;
    
    Matrix4x4 GetModelMatrix(CoordSpace space = CoordSpace::WORLD) const;
    Vector3 GetWorldPosition() const;
    Vector3 GetForwardVector() const;
    Vector3 GetRightVector() const;
    Vector3 GetUpVector() const;

    void SetPosition(const Vector3& newPosition);
    void SetRotation(const Quaternion& newRotation);
    void SetRotationEuler(const Vector3& newRotation);
    void SetScale(const Vector3& newScale);

private:
    void UpdateLocalModelMatrix();
};