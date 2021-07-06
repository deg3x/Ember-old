#include "Transform.h"

Transform::Transform()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Transform::Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale)
{
	this->position = initPosition;
	this->rotation = initRotation;
	this->scale = initScale;
}

Transform::~Transform()
{

}

glm::vec3 Transform::GetForwardVector() const
{
	glm::vec3 forward;

	forward.x = glm::cos(glm::radians(this->rotation.y)) * glm::cos(glm::radians(this->rotation.x));
	forward.y = glm::sin(glm::radians(this->rotation.x));
	forward.z = glm::sin(glm::radians(this->rotation.y)) * glm::cos(glm::radians(this->rotation.x));

	return glm::normalize(forward);
}

glm::vec3 Transform::GetRightVector() const
{
	return glm::normalize(glm::cross(this->GetForwardVector(), Transform::worldUp));
}

glm::vec3 Transform::GetUpVector() const
{
	return glm::normalize(glm::cross(Transform::worldRight, this->GetForwardVector()));
}

const glm::vec3 Transform::worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 Transform::worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);