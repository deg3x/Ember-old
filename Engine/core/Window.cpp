#include "Window.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

#include "logger/Logger.h"

GLFWwindow* Window::window;
WindowData Window::windowData;
MouseData Window::mouseData;

void Window::Initialize()
{
	windowData.title       = "Graphics Engine";
	windowData.windowW     = 1024;
	windowData.windowH     = 768;
	windowData.aspectRatio = 1024.0f / 768.0f;

	mouseData.sensitivity  = 0.01f;

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

void Window::MouseButtonCallback()
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!mouseData.leftButtonPressed)
		{
			glfwGetCursorPos(window, &mouseData.lastMouseX, &mouseData.lastMouseY);
		}

		mouseData.leftButtonPressed = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		mouseData.leftButtonPressed = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (!mouseData.rightButtonPressed)
		{
			glfwGetCursorPos(window, &mouseData.lastMouseX, &mouseData.lastMouseY);
		}

		mouseData.rightButtonPressed = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		mouseData.rightButtonPressed = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (!mouseData.middleButtonPressed)
		{
			glfwGetCursorPos(window, &mouseData.lastMouseX, &mouseData.lastMouseY);
		}

		mouseData.middleButtonPressed = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		mouseData.middleButtonPressed = false;
	}
}

void Window::MousePositionCallback()
{
	double mouseX;
	double mouseY;

	glfwGetCursorPos(window, &mouseX, &mouseY);
	
	if (glm::epsilonEqual(mouseX, mouseData.lastMouseX, DBL_EPSILON) && glm::epsilonEqual(mouseY, mouseData.lastMouseY, DBL_EPSILON))
	{
		return;
	}

	if (mouseData.leftButtonPressed)
	{
		mouseData.leftMouseXOffset = mouseX - mouseData.lastMouseX;
		mouseData.leftMouseYOffset = mouseY - mouseData.lastMouseY;
		mouseData.lastMouseX = mouseX;
		mouseData.lastMouseY = mouseY;
	}
	else
	{
		mouseData.leftMouseXOffset = 0.0;
		mouseData.leftMouseYOffset = 0.0;
	}

	if (mouseData.rightButtonPressed)
	{
		mouseData.rightMouseXOffset = mouseX - mouseData.lastMouseX;
		mouseData.rightMouseYOffset = mouseY - mouseData.lastMouseY;
		mouseData.lastMouseX = mouseX;
		mouseData.lastMouseY = mouseY;
	}
	else
	{
		mouseData.rightMouseXOffset = 0.0;
		mouseData.rightMouseYOffset = 0.0;
	}
}

void Window::ProcessUserInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	MouseButtonCallback();
	MousePositionCallback();
	
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::ResetMouseOffsetData()
{
	mouseData.leftMouseXOffset = 0.0;
	mouseData.leftMouseYOffset = 0.0;
	mouseData.rightMouseXOffset = 0.0;
	mouseData.rightMouseYOffset = 0.0;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}
