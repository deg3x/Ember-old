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
    static const glm::vec3 worldForward;
    static const glm::vec3 worldRight;
    static const glm::vec3 worldUp;
    
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 pivotOffset;

private:
    bool isModelUpdated = false;
    glm::mat4x4 localModelMatrix = glm::mat4x4(1.0f);
    
public:
    Transform();
    Transform(const glm::vec3& initPosition, const glm::quat& initRotation, const glm::vec3& initScale);
    Transform(const glm::vec3& initPosition, const glm::quat& initRotation, const glm::vec3& initScale, const glm::vec3& initPivotOffset);
    virtual ~Transform() = default;

    void Tick() override;

    void Translate(const glm::vec3& translateValue);
    void Rotate(const glm::quat& rotateValue);
    void Scale(const glm::vec3& scaleValue);

    inline glm::vec3 GetPosition() const
    {
        return position;
    }

    inline glm::quat GetRotation() const
    {
        return rotation;
    }
    
    inline glm::vec3 GetRotationEuler() const
    {
        return glm::degrees(glm::eulerAngles(rotation));
    }

    inline glm::vec3 GetScale() const
    {
        return scale;
    }
    
    glm::mat4x4 GetModelMatrix(CoordSpace space = CoordSpace::WORLD) const;
    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetForwardVector() const;
    glm::vec3 GetRightVector() const;
    glm::vec3 GetUpVector() const;

    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::quat& newRotation);
    void SetRotationEuler(const glm::vec3& newRotation);
    void SetScale(const glm::vec3& newScale);

private:
    void UpdateLocalModelMatrix();
};