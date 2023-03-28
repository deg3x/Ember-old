#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ApplicationWindow.h"
#include "core/Camera.h"
#include "utils/Sphere.h"
#include "core/Shader.h"
#include "utils/Plane.h"
#include "core/PointLight.h"
#include "core/DirectionalLight.h"
#include "core/SpotLight.h"

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
	Sphere sphere(32, 32, 1.0f);
	Transform sphereTransform;

	Plane plane(10, 1.0f);
	Transform planeTransform(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f));

	PointLight pLight;
	DirectionalLight dLight;
	SpotLight sLight;

	Shader shader("./Engine/shaders/vertexPhong.shader", "./Engine/shaders/fragmentPhong.shader");

	float theta = -glm::quarter_pi<float>();
	float phi = -glm::half_pi<float>();
	float radius = camera.transform.position.length();

	while (!glfwWindowShouldClose(appWindow->GetWindow()))
	{
		appWindow->ProcessUserInput();

		MouseData mouse = appWindow->GetMouseData();
		//camera.transform.position = camera.transform.position - (float)mouse.rightMouseYOffset * camera.transform.GetForwardVector() * mouse.sensitivity;

		theta += mouse.leftMouseXOffset * mouse.sensitivity;
		phi += mouse.leftMouseYOffset * mouse.sensitivity;
		radius -= (float)mouse.rightMouseYOffset * mouse.sensitivity;
		
		camera.transform.position.x = radius * glm::cos(theta) * glm::sin(phi);
		camera.transform.position.z = radius * glm::sin(theta) * glm::sin(phi);
		camera.transform.position.y = radius * glm::cos(phi);

		glm::mat4x4 viewMat = glm::lookAt(camera.transform.position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		
		appWindow->ResetMouseOffsetData();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 model = sphereTransform.GetModelMatrix();
		glm::mat4x4 view = viewMat;//camera.GetViewMatrix();
		glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));

		shader.SetVector3("cameraPosition", camera.transform.position);
		shader.SetMatrix4x4("model", model);
		shader.SetMatrix4x4("view", view);
		shader.SetMatrix4x4("projection", proj);
		shader.SetMatrix4x4("normalMatrix", normalMatrix);
		shader.SetVector3("material.diffuse", glm::vec3(0.9f, 0.3f, 0.4f));
		shader.SetVector3("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
		shader.SetFloat("material.shininess", 64);

		//pLight.transform.position.x = glm::sin(glfwGetTime() * 3.0f);
		//pLight.transform.position.z = glm::cos(glfwGetTime() * 3.0f);
		//pLight.SetShaderProperties(shader);

		dLight.transform.rotation.x = 30.0f;
		dLight.transform.rotation.y = -30.0f;
		dLight.SetShaderProperties(shader);
		
		//sLight.transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
		//sLight.transform.rotation.x = 90.0f;
		//sLight.transform.rotation.y = 90.0f;
		//sLight.SetShaderProperties(shader);

		shader.Use();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sphere.DrawMesh();

		model = planeTransform.GetModelMatrix();
		shader.SetMatrix4x4("model", model);
		normalMatrix = glm::transpose(glm::inverse(model));
		shader.SetMatrix4x4("normalMatrix", normalMatrix);
		plane.DrawMesh();

		glfwSwapBuffers(appWindow->GetWindow());
		glfwPollEvents();
	}
}