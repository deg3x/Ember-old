#include "engine_pch.h"
#include "Transform.h"

#include "core/Object.h"

Transform::Transform()
{
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    scale    = Vector3(1.0f, 1.0f, 1.0f);
    pivot    = Vector3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(const Vector3& initPosition, const Quaternion& initRotation, const Vector3& initScale)
{
    position = initPosition;
    rotation = initRotation;
    scale    = initScale;
    pivot    = Vector3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(const Vector3& initPosition, const Quaternion& initRotation, const Vector3& initScale, const Vector3& initPivotOffset)
{
    position = initPosition;
    rotation = initRotation;
    scale    = initScale;
    pivot    = initPivotOffset;
}

void Transform::Tick()
{
    Component::Tick();

    if (isModelUpdated)
    {
        return;
    }

    isModelUpdated = true;
    UpdateLocalModelMatrix();
}

void Transform::Translate(const Vector3& translateValue)
{
    if (translateValue.IsZero(0.001f))
    {
        return;
    }

    position += translateValue;
    isModelUpdated = false;
}

void Transform::Rotate(const Quaternion& rotateValue)
{
    if (rotateValue.IsZero(0.001f))
    {
        return;
    }

    rotation = rotateValue * rotation;
    rotation = rotation.Renormalize();
    isModelUpdated = false;
}

void Transform::Scale(const Vector3& scaleValue)
{
    if (scaleValue.IsZero(0.001f))
    {
        return;
    }

    scale *= scaleValue;
    isModelUpdated = false;
}

Matrix4x4 Transform::GetModelMatrix(CoordSpace space) const
{
    if (owner == nullptr || owner->GetParent() == nullptr || space == CoordSpace::LOCAL)
    {
        return localModelMatrix;
    }

    return owner->GetParent()->transform->localModelMatrix * localModelMatrix;
}

Vector3 Transform::GetWorldPosition() const
{
    const Matrix4x4 model = GetModelMatrix();

    return {model[3][0], model[3][1], model[3][2]};
}

Vector3 Transform::GetForwardVector() const
{
    return (rotation * WorldForward).Normalize();
}

Vector3 Transform::GetRightVector() const
{
    return (rotation * -WorldRight).Normalize();
}

Vector3 Transform::GetUpVector() const
{
    return (rotation * WorldUp).Normalize();
}

void Transform::SetPosition(const Vector3& newPosition)
{
    if (position.IsEqual(newPosition, 0.001f))
    {
        return;
    }

    position = newPosition;
    isModelUpdated = false;
}

void Transform::SetRotation(const Quaternion& newRotation)
{
    if (rotation.IsEqual(newRotation, 0.001f))
    {
        return;
    }

    rotation = newRotation;
    isModelUpdated = false;
}

void Transform::SetRotationEuler(const Vector3& newRotation)
{
    const Quaternion rotQuat = Quaternion::FromEuler(newRotation * DEG2RAD);
    
    if (rotation.IsEqual(rotQuat, 0.001f))
    {
        return;
    }

    rotation = rotQuat;
    isModelUpdated = false;
}

void Transform::SetScale(const Vector3& newScale)
{
    if (scale.IsEqual(newScale, 0.001f))
    {
        return;
    }

    scale = newScale;
    isModelUpdated = false;
}

void Transform::UpdateLocalModelMatrix()
{
    localModelMatrix = Matrix4x4::Model(position, rotation, scale, pivot);
}

const Vector3 Transform::WorldForward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Transform::WorldRight = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Transform::WorldUp = Vector3(0.0f, 1.0f, 0.0f);