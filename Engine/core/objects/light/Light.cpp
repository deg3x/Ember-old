#include "Light.h"

#include "glm/glm.hpp"

Light::Light() : Object()
{
	intensity = 1.0f;
	diffuse = glm::vec3(1.0f, 0.97f, 0.92f);
	ambient = glm::vec3(0.05f, 0.05f, 0.05f);
}
