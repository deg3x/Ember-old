#pragma once

#include "core/components/Component.h"

class ENGINE_API Transform : public Component
{
	MAKE_UNIQUE_COMPONENT()
	MAKE_COMPONENT_TYPE(TRANSFORM)
	
public:
	glm::vec3 position;
	glm::vec3 rotation; // Switch to quaternions later
	glm::vec3 scale;
	glm::vec3 pivotOffset;

	std::shared_ptr<Transform> parent = nullptr;
	std::vector<Transform> children;

	static const glm::vec3 worldForward;
	static const glm::vec3 worldRight;
	static const glm::vec3 worldUp;
	
public:
	Transform();
	Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale);
	Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale, glm::vec3 initPivotOffset);
	virtual ~Transform() = default;

	glm::mat4x4 GetModelMatrix() const;
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
};