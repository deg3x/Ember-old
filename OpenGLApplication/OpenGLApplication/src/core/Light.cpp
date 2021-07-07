#include "Light.h"
#include <glm/glm.hpp>

Light::Light()
{
	this->transform = Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->intensity = 1.0f;
	this->diffuse = glm::vec3(1.0f, 0.97f, 0.92f);
	this->ambient = glm::vec3(0.98f, 0.90f, 0.85f);
}

Light::~Light()
{

}