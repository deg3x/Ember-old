#include "Window.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/gtc/epsilon.hpp"

#include "logger/Logger.h"

GLFWwindow* Window::window;
WindowData Window::windowData;

void Window::Initialize()
{
	windowData.title       = "Graphics Engine";
	windowData.windowW     = 1024;
	windowData.windowH     = 768;
	windowData.aspectRatio = 1024.0f / 768.0f;

	if (!glfwInit())
	{
		Logger::LogError("Failed to initialize GLFW", "Window::Initialize");
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(windowData.windowW, windowData.windowH, windowData.title, NULL, NULL);
	if (window == NULL)
	{
		Logger::LogError("Failed to create GLFW window", "Window::Initialize");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}
