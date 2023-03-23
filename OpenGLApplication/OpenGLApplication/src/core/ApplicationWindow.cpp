#include <glad/glad.h>
#include <iostream>

#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(const int windowW, const int windowH, const char* windowName)
{
	windowData.windowW = windowW;
	windowData.windowH = windowH;
	windowData.aspectRatio = (float)windowW / (float)windowH;

	if (!(InitGLFW(windowName) + InitGLAD() + InitOpenGL()))
	{
		return;
	}
}

ApplicationWindow::~ApplicationWindow()
{
	glfwTerminate();
	
	window = nullptr;
	mainLoopCallback = nullptr;
}

int ApplicationWindow::InitGLFW(const char* windowName)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(windowData.windowW, windowData.windowH, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create window!" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	return 0;
}

int ApplicationWindow::InitGLAD() const
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	return 0;
}

int ApplicationWindow::InitOpenGL() const
{
	glViewport(0, 0, windowData.windowW, windowData.windowH);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void ApplicationWindow::MouseButtonCallback()
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

void ApplicationWindow::MousePositionCallback()
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
		mouseData.ResetLeftMouseOffsetData();
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
		mouseData.ResetRightMouseOffsetData();
	}
}

void ApplicationWindow::ProcessUserInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	MouseButtonCallback();
	MousePositionCallback();
}

int ApplicationWindow::MainLoop()
{
	if (mainLoopCallback != nullptr)
	{
		mainLoopCallback(this);
	}

	return 0;
}

void ApplicationWindow::SetMainLoopCallback(void (*callback)(ApplicationWindow* appWindow))
{
	mainLoopCallback = callback;
}

void ApplicationWindow::SetClearColor(const float r, const float g, const float b, const float a)
{
	windowData.clearColor.r = r;
	windowData.clearColor.g = g;
	windowData.clearColor.b = b;
	windowData.clearColor.a = a;
}

void ApplicationWindow::SetClearColor(const Color c)
{
	windowData.clearColor = c;
}

void ApplicationWindow::ResetMouseOffsetData()
{
	mouseData.ResetLeftMouseOffsetData();
	mouseData.ResetRightMouseOffsetData();
}

GLFWwindow* ApplicationWindow::GetWindow() const
{
	return window;
}

GLsizei ApplicationWindow::GetWindowWidth() const
{
	return windowData.windowW;
}

GLsizei ApplicationWindow::GetWindowHeight() const
{
	return windowData.windowH;
}

float ApplicationWindow::GetAspectRatio() const
{
	return windowData.aspectRatio;
}