#include "Transform.h"

Transform::Transform()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale)
{
	this->position = initPosition;
	this->rotation = initRotation;
	this->scale = initScale;
	this->pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(glm::vec3 initPosition, glm::vec3 initRotation, glm::vec3 initScale, glm::vec3 initPivotOffset)
{
	this->position = initPosition;
	this->rotation = initRotation;
	this->scale = initScale;
	this->pivotOffset = initPivotOffset;
}

Transform::~Transform()
{

}

glm::mat4x4 Transform::GetModelMatrix() const
{
	glm::mat4x4 model = glm::mat4x4(1.0f);

	model = glm::translate(model, this->position);

	model = glm::translate(model, this->pivotOffset);
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -this->pivotOffset);

	model = glm::scale(model, this->scale);

	return model;
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