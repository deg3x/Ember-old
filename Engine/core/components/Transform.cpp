#include "engine_pch.h"
#include "Transform.h"

#include "core/Object.h"

Transform::Transform()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale)
{
    position = initPosition;
    rotation = initRotation;
    scale = initScale;
    pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale, glm::vec3 initPivotOffset)
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

void Transform::SetPosition(const glm::vec3& newPosition)
{
    if (glm::all(glm::epsilonEqual(position, newPosition, 0.001f)))
    {
        return;
    }

    position = newPosition;
    isModelUpdated = false;
}

void Transform::SetRotation(const glm::vec3& newRotation)
{
    if (glm::all(glm::epsilonEqual(rotation, newRotation, 0.001f)))
    {
        return;
    }

    rotation = newRotation;
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

glm::mat4x4 Transform::GetModelMatrix() const
{
    if (owner == nullptr || owner->GetParent() == nullptr)
    {
        return localModelMatrix;
    }

    return owner->GetParent()->transform->localModelMatrix * localModelMatrix;
}

glm::vec3 Transform::GetForwardVector() const
{
    glm::vec3 forward;

    forward.x = glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x));
    forward.y = glm::sin(glm::radians(rotation.x));
    forward.z = glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x));

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

void Transform::UpdateLocalModelMatrix()
{
    glm::mat4x4 model = glm::mat4x4(1.0f);

    model = glm::translate(model, position);

    model = glm::translate(model, pivotOffset);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, -pivotOffset);

    model = glm::scale(model, scale);

    localModelMatrix = model;
}

const glm::vec3 Transform::worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 Transform::worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);