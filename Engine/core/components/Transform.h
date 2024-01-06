#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "core/components/Component.h"

class ENGINE_API Transform : public Component
{
public:
	MAKE_UNIQUE_COMPONENT()
	
	glm::vec3 position;
	glm::vec3 rotation; // Switch to quaternions later
	glm::vec3 scale;
	glm::vec3 pivotOffset;

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