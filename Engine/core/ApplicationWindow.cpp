#include "ApplicationWindow.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <iostream>

ApplicationWindow::ApplicationWindow(int windowW, int windowH, const char* windowName)
{
	windowData.windowW = windowW;
	windowData.windowH = windowH;
	windowData.aspectRatio = (float)windowW / (float)windowH;

	if (InitGLFW(windowName) + InitGLAD() + InitOpenGL() != 0)
	{
		// Initialization failed
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

int ApplicationWindow::InitOpenGL()
{
    // For some reason OSX requires double the window dimensions
#if defined(_WIN32)
	glViewport(0, 0, windowData.windowW, windowData.windowH);
#elif __APPLE__
    glViewport(0, 0, 2 * windowData.windowW, 2 * windowData.windowH);
#endif

	glPolygonMode(GL_FRONT, GL_FILL);

	windowData.clearBits = GL_COLOR_BUFFER_BIT;
	
	SetDepthTestEnabled(true);
	SetStencilTestEnabled(false);
	UpdateClearColor();

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

void ApplicationWindow::UpdateClearColor() const
{
	glClearColor(windowData.clearColor.r, windowData.clearColor.g, windowData.clearColor.b, windowData.clearColor.a);
}

void ApplicationWindow::ProcessUserInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	MouseButtonCallback();
	MousePositionCallback();
	
	glfwPollEvents();
}

void ApplicationWindow::Clear() const
{
	glClear(windowData.clearBits);
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

void ApplicationWindow::SetClearColor(float r, float g, float b, float a)
{
	windowData.clearColor.r = r;
	windowData.clearColor.g = g;
	windowData.clearColor.b = b;
	windowData.clearColor.a = a;

	UpdateClearColor();
}

void ApplicationWindow::SetClearColor(const Color& c)
{
	windowData.clearColor = c;
	
	UpdateClearColor();
}

void ApplicationWindow::SetDepthTestEnabled(bool state)
{
	if (state)
	{
		glEnable(GL_DEPTH_TEST);
		windowData.clearBits |= GL_DEPTH_BUFFER_BIT;
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		windowData.clearBits &= (GL_DEPTH_BUFFER_BIT ^ 0xFFFFFFFF);
	}
}

void ApplicationWindow::SetStencilTestEnabled(bool state)
{
	if (state)
	{
		glEnable(GL_STENCIL_TEST);
		windowData.clearBits |= GL_STENCIL_BUFFER_BIT;
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
		windowData.clearBits &= (GL_STENCIL_BUFFER_BIT ^ 0xFFFFFFFF);
	}
}

void ApplicationWindow::SetDepthTestMask(bool mask)
{
	glDepthMask(mask);
}

void ApplicationWindow::SetDepthTestFunc(unsigned int func)
{
	glDepthFunc(func);
}

void ApplicationWindow::SetStencilTestMask(unsigned int mask)
{
	glStencilMask(mask);
}

void ApplicationWindow::SetStencilTestFunc(unsigned int func, int reference, unsigned int mask)
{
	glStencilFunc(func, reference, mask);
}

void ApplicationWindow::SetStencilTestOp(unsigned int stencilFail, unsigned int depthFail, unsigned int depthPass)
{
	glStencilOp(stencilFail, depthFail, depthPass);
}

void ApplicationWindow::SwapBuffers() const
{
	glfwSwapBuffers(window);
}

void ApplicationWindow::ResetMouseOffsetData()
{
	mouseData.ResetLeftMouseOffsetData();
	mouseData.ResetRightMouseOffsetData();
}

bool ApplicationWindow::ShouldClose() const
{
	return glfwWindowShouldClose(window);
}
