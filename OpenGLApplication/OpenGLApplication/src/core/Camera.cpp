#include "Camera.h"

Camera::Camera()
{
	transform = Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

Camera::~Camera()
{

}