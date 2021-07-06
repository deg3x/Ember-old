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

	Sphere sphere(32, 32, 1.0f);
	Mesh sphereMesh(sphere.GetVertexData(), sphere.GetIndices());
	Transform sphereTransform;

	Shader shader("./src/shaders/vertexPhong.shader", "./src/shaders/fragmentPhong.shader");

	while (!glfwWindowShouldClose(appWindow->GetWindow()))
	{
		appWindow->ProcessUserInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 model = glm::mat4x4(1.0f);
		glm::mat4x4 view = glm::mat4x4(1.0f);
		glm::mat4x4 proj = glm::mat4x4(1.0f);

		model = glm::translate(model, sphereTransform.position);

		model = glm::rotate(model, glm::radians(sphereTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(sphereTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(sphereTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, sphereTransform.scale);

		Camera cam;
		cam.transform = Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		view = cam.GetViewMatrix();
		proj = glm::perspective(glm::radians(90.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

		glm::vec3 lightPos = glm::vec3(10.0f, 10.0f, 10.0f);
		glm::vec4 lightColor = glm::vec4(0.9f, 0.9f, 0.8f, 1.0f);

		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));

		shader.SetUniform4f("color", glm::vec4(0.9f, 0.3f, 0.4f, 1.0f));
		shader.SetUniform3f("cameraPos", cam.transform.position);
		shader.SetUniform4f("lightColor", lightColor);
		shader.SetUniform3f("lightPos", lightPos);
		shader.SetMatrix4fv("model", model);
		shader.SetMatrix4fv("view", view);
		shader.SetMatrix4fv("projection", proj);
		shader.SetMatrix4fv("normalMatrix", normalMatrix);

		shader.Use();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sphereMesh.DrawMesh();

		glfwSwapBuffers(appWindow->GetWindow());
		glfwPollEvents();
	}
}