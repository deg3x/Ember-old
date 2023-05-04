#include <iostream>

#include "Definitions.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "core/components/lights/DirectionalLight.h"
#include "core/objects/Object.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "core/components/meshes/Sphere.h"
#include "core/components/meshes/Plane.h"
#include "core/components/meshes/Cube.h"
#include "core/ApplicationWindow.h"
#include "core/Material.h"
#include "core/Scene.h"

void MainWindowCallback(ApplicationWindow* appWindow);

int main()
{
	ApplicationWindow app(1024, 768, "Test Application");

	app.SetMainLoopCallback(MainWindowCallback);
	app.MainLoop();

	return 0;
}

void MainWindowCallback(ApplicationWindow* appWindow)
{
	Scene scene;
	scene.Use();

	const std::shared_ptr<Object> cameraObject = std::make_shared<Object>();
	cameraObject->CreateComponent<Camera>();
	cameraObject->transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraObject->transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);

	const std::shared_ptr<Material> sphereMat = std::make_shared<Material>();

	const std::shared_ptr<Object> sphereObject = std::make_shared<Object>();
	sphereObject->CreateComponent<Sphere>(32, 32, 0.5f, sphereMat);
	sphereObject->transform->position = glm::vec3(-0.8f, 0.0f, -0.8f);

	const std::shared_ptr<Object> planeObject = std::make_shared<Object>();
	planeObject->CreateComponent<Plane>(10, 3.0f, sphereMat);
	planeObject->transform->position = glm::vec3(0.0f, -0.5f, 0.0f);

	const std::shared_ptr<Object> cubeObject = std::make_shared<Object>();
	cubeObject->CreateComponent<Cube>(sphereMat);
	cubeObject->transform->position = glm::vec3(0.8f, 0.0f, -0.8f);

	const std::shared_ptr<Object> bunnyObject = std::make_shared<Object>();
	bunnyObject->transform->position = glm::vec3(0.4f, -0.33f, 0.5f);
	bunnyObject->transform->scale =glm::vec3(0.5f, 0.5f, 0.5f);
	bunnyObject->LoadModel("./Data/models/bunny.obj");

	const std::shared_ptr<Object> dirLightObject = std::make_shared<Object>();
	dirLightObject->CreateComponent<DirectionalLight>();
	dirLightObject->transform->rotation.x = 30.0f;
	dirLightObject->transform->rotation.y = -30.0f;

	dirLightObject->GetComponent<Light>()->SetShaderProperties(*sphereMat->GetShader());

	scene.AddObject(cameraObject);
	scene.AddObject(sphereObject);
	scene.AddObject(planeObject);
	scene.AddObject(cubeObject);
	scene.AddObject(bunnyObject);
	scene.AddObject(dirLightObject);

	float theta  = -glm::quarter_pi<float>();
	float phi    = -glm::half_pi<float>();
	float radius = (float)cameraObject->transform->position.length();

	ApplicationWindow::SetDepthTestFunc(GL_LESS);

	while (!appWindow->ShouldClose())
	{
		appWindow->ProcessUserInput();

		const MouseData mouse = appWindow->GetMouseData();

		theta  += (float)mouse.leftMouseXOffset * mouse.sensitivity;
		phi	   += (float)mouse.leftMouseYOffset * mouse.sensitivity;
		radius -= (float)mouse.rightMouseYOffset * mouse.sensitivity;

		cameraObject->transform->position.x = radius * glm::cos(theta) * glm::sin(phi);
		cameraObject->transform->position.z = radius * glm::sin(theta) * glm::sin(phi);
		cameraObject->transform->position.y = radius * glm::cos(phi);

		appWindow->ResetMouseOffsetData();
		appWindow->Clear();

		scene.Tick();

		appWindow->SwapBuffers();
	}
}