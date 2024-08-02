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

void Transform::Translate(const Vector3& translateValue, CoordSpace space)
{
    if (translateValue.IsZero(0.001f))
    {
        return;
    }

    Vector3 translate = translateValue;
    
    if (space == CoordSpace::WORLD && owner->GetParent() != nullptr)
    {
        const Quaternion parentRot = owner->GetParent()->transform->GetRotation(CoordSpace::WORLD);
        translate = parentRot.Conjugate() * translateValue;
    }

    position += translate;
    isModelUpdated = false;
}

void Transform::Rotate(const Quaternion& rotateValue, CoordSpace space)
{
    if (rotateValue.IsZero(0.001f))
    {
        return;
    }

    Quaternion rotate = rotateValue;
    
    if (space == CoordSpace::WORLD && owner->GetParent() != nullptr)
    {
        const Quaternion parentRot = owner->GetParent()->transform->GetRotation(CoordSpace::WORLD);
        rotate = parentRot.Conjugate() * rotateValue * parentRot;
    }

    rotation = rotate * rotation;
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

Vector3 Transform::GetPosition(CoordSpace space) const
{
    if (space == CoordSpace::LOCAL)
    {
        return position;
    }

    const std::shared_ptr<Object> parent = owner->GetParent();
    if (parent == nullptr)
    {
        return position;
    }

    return parent->transform->GetPosition(CoordSpace::WORLD) + position;
}

Quaternion Transform::GetRotation(CoordSpace space) const
{
    if (space == CoordSpace::LOCAL)
    {
        return rotation;
    }
    
    const std::shared_ptr<Object> parent = owner->GetParent();
    if (parent == nullptr)
    {
        return rotation;
    }

    return parent->transform->GetRotation(CoordSpace::WORLD) * rotation;
}

Vector3 Transform::GetRotationEuler(CoordSpace space) const
{
    return GetRotation(space).ToEuler() * RAD2DEG;
}

Vector3 Transform::GetScale(CoordSpace space) const
{
    if (space == CoordSpace::LOCAL)
    {
        return scale;
    }
    
    const std::shared_ptr<Object> parent = owner->GetParent();
    if (parent == nullptr)
    {
        return scale;
    }

    return parent->transform->GetScale(CoordSpace::WORLD) * scale;
}

Matrix4x4 Transform::GetModelMatrix(CoordSpace space) const
{
    if (owner == nullptr || owner->GetParent() == nullptr || space == CoordSpace::LOCAL)
    {
        return localModelMatrix;
    }

    return owner->GetParent()->transform->GetModelMatrix() * localModelMatrix;
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