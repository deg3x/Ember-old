#include "Light.h"
#include <glm/glm.hpp>

Light::Light()
{
	this->transform = Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->intensity = 1.0f;
}

Light::~Light()
{

}