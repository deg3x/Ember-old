#include <glad/glad.h>
#include <iostream>

#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(const int windowW, const int windowH, const char* windowName) : windowW(windowW), windowH(windowH)
{
	this->aspectRatio = (float)windowW / (float)windowH;

	if (!(this->InitGLFW(windowName) + this->InitGLAD() + this->InitOpenGL()))
	{
		return;
	}

	//glfwSetFramebufferSizeCallback(this->window, this->WindowResizeCallback);
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

	this->window = glfwCreateWindow(this->windowW, this->windowH, windowName, NULL, NULL);
	if (this->window == NULL)
	{
		std::cerr << "Failed to create window!" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(this->window);

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
	glViewport(0, 0, windowW, windowH);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void ApplicationWindow::ProcessUserInput() const
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, true);
	}
}

int ApplicationWindow::MainLoop() const
{
	if (this->mainLoopCallback != nullptr)
	{
		this->mainLoopCallback(this);
	}

	return 0;
}

void ApplicationWindow::SetMainLoopCallback(void (*callback)(const ApplicationWindow* appWindow))
{
	this->mainLoopCallback = callback;
}

void ApplicationWindow::SetClearColor(const float r, const float g, const float b, const float a)
{
	this->clearColor.r = r;
	this->clearColor.g = g;
	this->clearColor.b = b;
	this->clearColor.a = a;
}

void ApplicationWindow::SetClearColor(const Color c)
{
	this->clearColor = c;
}

GLFWwindow* ApplicationWindow::GetWindow() const
{
	return this->window;
}

GLsizei ApplicationWindow::GetWindowWidth() const
{
	return this->windowW;
}

GLsizei ApplicationWindow::GetWindowHeight() const
{
	return this->windowH;
}

float ApplicationWindow::GetAspectRatio() const
{
	return this->aspectRatio;
}