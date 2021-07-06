#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ApplicationWindow.h"
#include "core/Camera.h"
#include "utils/Sphere.h"
#include "core/Mesh.h"
#include "core/Shader.h"
#include "utils/Plane.h"

void MainWindowCallback(const ApplicationWindow* appWindow);

int main()
{
	ApplicationWindow app(1024, 768, "Test Application");

	app.SetMainLoopCallback(MainWindowCallback);
	app.MainLoop();

	return 0;
}

void MainWindowCallback(const ApplicationWindow* appWindow)
{
	glm::vec4 clearColor = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

	Camera camera;
	Sphere sphere(32, 32, 1.0f);
	Mesh sphereMesh(sphere.GetVertexData(), sphere.GetIndices());
	Transform sphereTransform;

	Plane plane(10, 1.0f);
	Mesh planeMesh(plane.GetVertexData(), plane.GetIndices());
	Transform planeTransform(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f));

	Shader shader("./src/shaders/vertexPhong.shader", "./src/shaders/fragmentPhong.shader");

	while (!glfwWindowShouldClose(appWindow->GetWindow()))
	{
		appWindow->ProcessUserInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 model = sphereTransform.GetModelMatrix();
		glm::mat4x4 view = camera.GetViewMatrix();
		glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

		glm::vec3 lightPos = glm::vec3(10.0f, 10.0f, 10.0f);
		glm::vec4 lightColor = glm::vec4(0.9f, 0.9f, 0.8f, 1.0f);

		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));

		shader.SetUniform4f("color", glm::vec4(0.9f, 0.3f, 0.4f, 1.0f));
		shader.SetUniform3f("cameraPos", camera.transform.position);
		shader.SetUniform4f("lightColor", lightColor);
		shader.SetUniform3f("lightPos", lightPos);
		shader.SetMatrix4fv("model", model);
		shader.SetMatrix4fv("view", view);
		shader.SetMatrix4fv("projection", proj);
		shader.SetMatrix4fv("normalMatrix", normalMatrix);

		shader.Use();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sphereMesh.DrawMesh();

		model = planeTransform.GetModelMatrix();
		shader.SetMatrix4fv("model", model);
		normalMatrix = glm::transpose(glm::inverse(model));
		shader.SetMatrix4fv("normalMatrix", normalMatrix);
		planeMesh.DrawMesh();

		glfwSwapBuffers(appWindow->GetWindow());
		glfwPollEvents();
	}
}