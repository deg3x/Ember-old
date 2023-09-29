#pragma once

#include "../Definitions.h"

struct WindowData
{
	const char* title;
	int windowW;
	int windowH;
	float aspectRatio;
};

struct MouseData
{
	float sensitivity;
	bool leftButtonPressed;
	bool rightButtonPressed;
	bool middleButtonPressed;
	double lastMouseX;
	double lastMouseY;
	double leftMouseXOffset;
	double leftMouseYOffset;
	double rightMouseXOffset;
	double rightMouseYOffset;
};

struct GLFWwindow;

class ENGINE_API Window
{
private:
	// Leave this as a raw pointer since we only manage it internally
	static GLFWwindow* window;
	static WindowData windowData;
	static MouseData mouseData;

public:
	Window() = delete;

	static void Initialize();

	static void ProcessUserInput();
	static void SwapBuffers();
	static void ResetMouseOffsetData();
	static bool ShouldClose();

	inline static WindowData GetWindowData()
	{
		return windowData;
	}

	inline static GLFWwindow* GetWindow()
	{
		return window;
	}

	inline static int GetWindowWidth()
	{
		return windowData.windowW;
	}

	inline static int GetWindowHeight()
	{
		return windowData.windowH;
	}

	inline static float GetAspectRatio()
	{
		return windowData.aspectRatio;
	}

	inline static MouseData GetMouseData() 
	{ 
		return mouseData; 
	}

private:
	static void MouseButtonCallback();
	static void MousePositionCallback();
};
