#pragma once
#include <GLFW64/glfw3.h>
#include "../utils/Types.h"

struct MouseData
{
	float sensitivity = 0.01f;
	bool leftButtonPressed;
	bool rightButtonPressed;
	bool middleButtonPressed;
	double lastMouseX;
	double lastMouseY;
	double leftMouseXOffset;
	double leftMouseYOffset;
	double rightMouseXOffset;
	double rightMouseYOffset;

	void ResetLeftMouseOffsetData()
	{
		leftMouseXOffset = 0.0;
		leftMouseYOffset = 0.0;
	}

	void ResetRightMouseOffsetData()
	{
		rightMouseXOffset = 0.0;
		rightMouseYOffset = 0.0;
	}
};

struct WindowData
{
	GLsizei windowW;
	GLsizei windowH;
	float aspectRatio;
	Color clearColor = { 0.12f, 0.12f, 0.12f, 1.0f };
};

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

	GLFWwindow* GetWindow() const;
	GLsizei GetWindowWidth() const;
	GLsizei GetWindowHeight() const;
	float GetAspectRatio() const;
	inline MouseData GetMouseData() const { return mouseData; }
};