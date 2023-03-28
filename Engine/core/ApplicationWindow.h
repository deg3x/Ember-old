#pragma once
#include <glfw/glfw3.h>
#include "../utils/Types.h"

class ApplicationWindow
{
private:
	GLFWwindow* window;
	WindowData windowData;
	MouseData mouseData;

	void (*mainLoopCallback)(ApplicationWindow* appWindow) = nullptr;

	int InitGLFW(const char* windowName);
	int InitGLAD() const;
	int InitOpenGL() const;
	
	void MouseButtonCallback();
	void MousePositionCallback();

public:
	ApplicationWindow() = delete;
	ApplicationWindow(const int windowW, const int windowH, const char* windowName);
	virtual ~ApplicationWindow();

	int MainLoop();
	void ProcessUserInput();

	void SetMainLoopCallback(void (*callback)(ApplicationWindow* appWindow));
	void SetClearColor(const float r, const float g, const float b, const float a);
	void SetClearColor(const Color c);

	void ResetMouseOffsetData();

	inline WindowData GetWindowData() const
	{
		return windowData;
	}

	inline GLFWwindow* GetWindow() const
	{
		return window;
	}

	inline GLsizei GetWindowWidth() const
	{
		return windowData.windowW;
	}

	inline GLsizei GetWindowHeight() const
	{
		return windowData.windowH;
	}

	float GetAspectRatio() const
	{
		return windowData.aspectRatio;
	}

	inline MouseData GetMouseData() const 
	{ 
		return mouseData; 
	}
};