#pragma once

#include "../utils/Types.h"

class GLFWwindow;

class ApplicationWindow
{
private:
	// Leave this as a raw pointer since we only manage it internally
	GLFWwindow* window;

	WindowData windowData;
	MouseData mouseData;

	void (*mainLoopCallback)(ApplicationWindow* appWindow) = nullptr;

public:
	ApplicationWindow() = delete;
	ApplicationWindow(const int windowW, const int windowH, const char* windowName);
	virtual ~ApplicationWindow();

	int MainLoop();
	void ProcessUserInput();

	void SetMainLoopCallback(void (*callback)(ApplicationWindow* appWindow));
	void SetClearColor(const float r, const float g, const float b, const float a);
	void SetClearColor(const Color c);
	void SwapBuffers() const;

	void ResetMouseOffsetData();
	bool ShouldClose() const;

	inline WindowData GetWindowData() const
	{
		return windowData;
	}

	inline GLFWwindow* GetWindow() const
	{
		return window;
	}

	inline int GetWindowWidth() const
	{
		return windowData.windowW;
	}

	inline int GetWindowHeight() const
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

private:
	int InitGLFW(const char* windowName);
	int InitGLAD() const;
	int InitOpenGL() const;
	
	void MouseButtonCallback();
	void MousePositionCallback();

	void EnableClearColor() const;
};
