#include "glad/glad.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "core/ApplicationWindow.h"
#include "core/Camera.h"
#include "utils/Sphere.h"
#include "core/Material.h"
#include "core/Shader.h"
#include "utils/Plane.h"
#include "core/PointLight.h"
#include "core/DirectionalLight.h"
#include "core/SpotLight.h"
#include "core/Model.h"
#include "core/Object.h"

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
	glm::vec4 clearColor = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

	Camera camera;

	Object sphereObject;
	sphereObject.CreateComponent<Sphere>(32, 32, 1.0f);
	sphereObject.CreateComponent<Material>();

	Object planeObject;
	planeObject.CreateComponent<Plane>(10, 1.0f);
	planeObject.AddComponent<Material>(sphereObject.GetComponent<Material>());
	planeObject.GetComponent<Transform>()->position = glm::vec3(0.0f, -1.0f, 0.0f);
	planeObject.GetComponent<Transform>()->rotation = glm::vec3(0.0f, 45.0f, 0.0f);
	planeObject.GetComponent<Transform>()->scale = glm::vec3(3.0f, 3.0f, 3.0f);

	Object bunnyObject;
	bunnyObject.AddComponent<Material>(sphereObject.GetComponent<Material>());
	bunnyObject.GetComponent<Transform>()->position = glm::vec3(0.0f, -0.8f, 0.0f);
	bunnyObject.GetComponent<Transform>()->scale =glm::vec3(0.6f, 0.6f, 0.6f);
#if defined(_WIN32)
	bunnyObject.CreateComponent<Model>("./Data/models/bunny.obj");
#elif __APPLE__
	bunnyObject.CreateComponent<Model>("../../Data/models/bunny.obj");
#endif

	DirectionalLight dLight;

	dLight.transform.rotation.x = 30.0f;
	dLight.transform.rotation.y = -30.0f;
	dLight.SetShaderProperties(*sphereObject.GetComponent<Material>()->GetShader());

	float theta = -glm::quarter_pi<float>();
	float phi = -glm::half_pi<float>();
	float radius = camera.transform.position.length();

	while (!glfwWindowShouldClose(appWindow->GetWindow()))
	{
		appWindow->ProcessUserInput();

		MouseData mouse = appWindow->GetMouseData();

		theta += mouse.leftMouseXOffset * mouse.sensitivity;
		phi += mouse.leftMouseYOffset * mouse.sensitivity;
		radius -= (float)mouse.rightMouseYOffset * mouse.sensitivity;
		
		camera.transform.position.x = radius * glm::cos(theta) * glm::sin(phi);
		camera.transform.position.z = radius * glm::sin(theta) * glm::sin(phi);
		camera.transform.position.y = radius * glm::cos(phi);

		glm::mat4x4 viewMat = glm::lookAt(camera.transform.position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		
		appWindow->ResetMouseOffsetData();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 model = sphereObject.GetComponent<Transform>()->GetModelMatrix();
		glm::mat4x4 view = viewMat; //camera.GetViewMatrix();
		glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));

		sphereObject.GetComponent<Material>()->GetShader()->SetVector3("cameraPosition", camera.transform.position);
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("model", model);
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("view", view);
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("projection", proj);
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("normalMatrix", normalMatrix);
		sphereObject.GetComponent<Material>()->GetShader()->SetVector3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		sphereObject.GetComponent<Material>()->GetShader()->SetVector3("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
		sphereObject.GetComponent<Material>()->GetShader()->SetFloat("material.shininess", 64);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//sphereObject.Draw();

		model = planeObject.GetComponent<Transform>()->GetModelMatrix();
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("model", model);
		normalMatrix = glm::transpose(glm::inverse(model));
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("normalMatrix", normalMatrix);
		planeObject.Draw();

		model = bunnyObject.GetComponent<Transform>()->GetModelMatrix();
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("model", model);
		normalMatrix = glm::transpose(glm::inverse(model));
		sphereObject.GetComponent<Material>()->GetShader()->SetMatrix4x4("normalMatrix", normalMatrix);
		bunnyObject.Draw();

		glfwSwapBuffers(appWindow->GetWindow());
		glfwPollEvents();
	}
}
