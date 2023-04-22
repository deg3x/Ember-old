#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "core/objects/light/DirectionalLight.h"
#include "core/objects/Object.h"
#include "core/objects/Camera.h"
#include "core/components/mesh/Sphere.h"
#include "core/components/mesh/Plane.h"
#include "core/components/mesh/Cube.h"
#include "core/ApplicationWindow.h"
#include "core/Material.h"
#include "core/Shader.h"

#include <iostream>

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
	Camera camera;
	
	std::shared_ptr<Material> sphereMat = std::make_shared<Material>();
	
	Object sphereObject;
	sphereObject.CreateComponent<Sphere>(32, 32, 0.5f, sphereMat);
	sphereObject.transform->position = glm::vec3(-0.5f, 0.0f, 0.0f);

	Object planeObject;
	planeObject.CreateComponent<Plane>(10, 2.0f, sphereMat);
	planeObject.transform->position = glm::vec3(0.0f, -0.5f, 0.0f);

	Object cubeObject;
	cubeObject.CreateComponent<Cube>(sphereMat);
	cubeObject.transform->position = glm::vec3(0.5f, 0.0f, 0.0f);

	Object bunnyObject;
	bunnyObject.transform->position = glm::vec3(0.0f, -0.8f, 0.0f);
	bunnyObject.transform->scale =glm::vec3(0.6f, 0.6f, 0.6f);
	bunnyObject.LoadModel("./Data/models/bunny.obj");
	
	DirectionalLight dLight;
	dLight.SetShaderProperties(*sphereMat->GetShader());

	float theta = -glm::quarter_pi<float>();
	float phi = -glm::half_pi<float>();
	float radius = camera.transform->position.length();

	while (!appWindow->ShouldClose())
	{
		appWindow->ProcessUserInput();

		MouseData mouse = appWindow->GetMouseData();

		theta += mouse.leftMouseXOffset * mouse.sensitivity;
		phi += mouse.leftMouseYOffset * mouse.sensitivity;
		radius -= (float)mouse.rightMouseYOffset * mouse.sensitivity;
		
		camera.transform->position.x = radius * glm::cos(theta) * glm::sin(phi);
		camera.transform->position.z = radius * glm::sin(theta) * glm::sin(phi);
		camera.transform->position.y = radius * glm::cos(phi);

		glm::mat4x4 viewMat = glm::lookAt(camera.transform->position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		
		appWindow->ResetMouseOffsetData();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 view = viewMat; //camera.GetViewMatrix();
		glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

		sphereMat->GetShader()->SetVector3("cameraPosition", camera.transform->position);
		sphereMat->GetShader()->SetMatrix4x4("view", view);
		sphereMat->GetShader()->SetMatrix4x4("projection", proj);
		sphereMat->GetShader()->SetVector3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		sphereMat->GetShader()->SetVector3("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
		sphereMat->GetShader()->SetFloat("material.shininess", 64);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sphereObject.Draw();
		planeObject.Draw();
		cubeObject.Draw();
		//bunnyObject.Draw();

		appWindow->SwapBuffers();
	}
}
