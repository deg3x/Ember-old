#pragma once
#include <GLFW64/glfw3.h>
#include "../utils/Types.h"

class ApplicationWindow
{
private:
	GLFWwindow* window;
	GLsizei windowW;
	GLsizei windowH;
	float aspectRatio;
	Color clearColor = { 0.12f, 0.12f, 0.12f, 1.0f };

	void (*mainLoopCallback)(const ApplicationWindow* appWindow) = nullptr;

	int InitGLFW(const char* windowName);
	int InitGLAD() const;
	int InitOpenGL() const;

public:
	ApplicationWindow() = delete;
	ApplicationWindow(const int windowW, const int windowH, const char* windowName);
	virtual ~ApplicationWindow();

	int MainLoop() const;
	void ProcessUserInput() const;

	void SetMainLoopCallback(void (*callback)(const ApplicationWindow* appWindow));
	void SetClearColor(const float r, const float g, const float b, const float a);
	void SetClearColor(const Color c);

	GLFWwindow* GetWindow() const;
	GLsizei GetWindowWidth() const;
	GLsizei GetWindowHeight() const;
	float GetAspectRatio() const;
};