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

	Shader shader("C:\\dev\\GraphicsPlayground\\OpenGLApplication\\OpenGLApplication\\src\\shaders\\vertexPhong.shader",
		"C:\\dev\\GraphicsPlayground\\OpenGLApplication\\OpenGLApplication\\src\\shaders\\fragmentPhong.shader");

	while (!glfwWindowShouldClose(appWindow->GetWindow()))
	{
		appWindow->ProcessUserInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Sphere sphere(32, 32, 1.0f);
		Mesh sphereMesh(sphere.GetVertexData(), sphere.GetIndices());

		glm::mat4x4 model = glm::mat4x4(1.0f);
		glm::mat4x4 view = glm::mat4x4(1.0f);
		glm::mat4x4 proj = glm::mat4x4(1.0f);

		Transform sphereTransform;

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
		glm::vec3 lightColor = glm::vec3(0.9f, 0.9f, 0.8f);

		GLuint lightPosLoc = glGetUniformLocation(shader.GetShaderID(), "lightPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		GLuint lightColorLoc = glGetUniformLocation(shader.GetShaderID(), "lightColor");
		glUniform4f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z, 1.0f);

		//shader.SetUniform4f("color", glm::vec4(0.9f, 0.3f, 0.4f, 1.0f));		// FIX THIS
		glm::vec4 color = glm::vec4(0.9f, 0.3f, 0.4f, 1.0f);
		GLuint colorLoc = glGetUniformLocation(shader.GetShaderID(), "color");
		glUniform4f(colorLoc, color.x, color.y, color.z, color.a);

		GLuint camPos = glGetUniformLocation(shader.GetShaderID(), "cameraPos");
		glUniform3f(camPos, cam.transform.position.x, cam.transform.position.y, cam.transform.position.z);

		//shader.SetUniform3f("cameraPos", cam.transform.position);				// AND THIS

		GLuint modelLoc = glGetUniformLocation(shader.GetShaderID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint viewLoc = glGetUniformLocation(shader.GetShaderID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projLoc = glGetUniformLocation(shader.GetShaderID(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));
		GLuint normLoc = glGetUniformLocation(shader.GetShaderID(), "normalMatrix");
		glUniformMatrix4fv(normLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		shader.Use();




		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sphereMesh.DrawMesh();

		glfwSwapBuffers(appWindow->GetWindow());
		glfwPollEvents();
	}
}