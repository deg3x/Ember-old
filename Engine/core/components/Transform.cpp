#include "engine_pch.h"
#include "Transform.h"

#include "core/Object.h"

Transform::Transform()
{
    position = glm::vec3();
    rotation = glm::quat();
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    pivotOffset = glm::vec3();
}

Transform::Transform(const glm::vec3& initPosition, const glm::quat& initRotation, const glm::vec3& initScale)
{
    position = initPosition;
    rotation = initRotation;
    scale = initScale;
    pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(const glm::vec3& initPosition, const glm::quat& initRotation, const glm::vec3& initScale, const glm::vec3& initPivotOffset)
{
    position = initPosition;
    rotation = initRotation;
    scale = initScale;
    pivotOffset = initPivotOffset;
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

void Transform::Translate(const glm::vec3& translateValue)
{
    if (glm::all(glm::epsilonEqual(translateValue, glm::vec3(), 0.001f)))
    {
        return;
    }

    position += translateValue;
    isModelUpdated = false;
}

void Transform::Rotate(const glm::quat& rotateValue)
{
    if (glm::all(glm::epsilonEqual(rotateValue, glm::quat(), 0.001f)))
    {
        return;
    }

    rotation *= rotateValue;
    isModelUpdated = false;
}

void Transform::Scale(const glm::vec3& scaleValue)
{
    if (glm::all(glm::epsilonEqual(scaleValue, glm::vec3(), 0.001f)))
    {
        return;
    }

    scale *= scaleValue;
    isModelUpdated = false;
}

glm::mat4x4 Transform::GetModelMatrix() const
{
    if (owner == nullptr || owner->GetParent() == nullptr)
    {
        return localModelMatrix;
    }

    return owner->GetParent()->transform->localModelMatrix * localModelMatrix;
}

glm::vec3 Transform::GetWorldPosition() const
{
    const glm::mat4x4 model = GetModelMatrix();

    return {model[3][0], model[3][1], model[3][2]};
}

glm::vec3 Transform::GetForwardVector() const
{
    const glm::vec3 rotEulerRad = glm::eulerAngles(rotation);

    glm::vec3 forward;

    forward.x = glm::cos(rotEulerRad.x) * glm::cos(glm::radians(rotEulerRad.x));
    forward.y = glm::sin(rotEulerRad.y);
    forward.z = glm::sin(rotEulerRad.z) * glm::cos(glm::radians(rotEulerRad.x));

    return glm::normalize(forward);
}

glm::vec3 Transform::GetRightVector() const
{
    return glm::normalize(glm::cross(GetForwardVector(), Transform::worldUp));
}

glm::vec3 Transform::GetUpVector() const
{
    return glm::normalize(glm::cross(GetRightVector(), GetForwardVector()));
}

void Transform::SetPosition(const glm::vec3& newPosition)
{
    if (glm::all(glm::epsilonEqual(position, newPosition, 0.001f)))
    {
        return;
    }

    position = newPosition;
    isModelUpdated = false;
}

void Transform::SetRotation(const glm::quat& newRotation)
{
    if (glm::all(glm::epsilonEqual(rotation, newRotation, 0.001f)))
    {
        return;
    }

    rotation = newRotation;
    isModelUpdated = false;
}

void Transform::SetRotationEuler(const glm::vec3& newRotation)
{
    const glm::quat rotQuat(glm::radians(newRotation));
    
    if (glm::all(glm::epsilonEqual(rotation, rotQuat, 0.001f)))
    {
        return;
    }

    rotation = rotQuat;
    isModelUpdated = false;
}

void Transform::SetScale(const glm::vec3& newScale)
{
    if (glm::all(glm::epsilonEqual(scale, newScale, 0.001f)))
    {
        return;
    }

    scale = newScale;
    isModelUpdated = false;
}

void Transform::UpdateLocalModelMatrix()
{
    const glm::vec3 rotEuler = glm::degrees(glm::eulerAngles(rotation));

    glm::mat4x4 model = glm::mat4x4(1.0f);

    model = glm::translate(model, position);

    model = glm::translate(model, pivotOffset);
    model = glm::rotate(model, glm::radians(rotEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, -pivotOffset);

    model = glm::scale(model, scale);

    localModelMatrix = model;
}

const glm::vec3 Transform::worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 Transform::worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);