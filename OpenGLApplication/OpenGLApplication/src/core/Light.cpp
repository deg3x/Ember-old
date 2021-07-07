#include "Light.h"
#include <glm/glm.hpp>

Light::Light()
{
	this->transform = Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->intensity = 1.0f;
	this->diffuse = glm::vec4(1.0f, 0.97f, 0.92f, 1.0f);
	this->ambient = glm::vec4(0.98f, 0.90f, 0.85f, 1.0f);
}

Light::~Light()
{

}