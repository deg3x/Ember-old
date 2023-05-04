#pragma once

#include "../Definitions.h"
#include "../utils/Types.h"

struct GLFWwindow;

class ENGINE_API ApplicationWindow
{
private:
	// Leave this as a raw pointer since we only manage it internally
	GLFWwindow* window;

	WindowData windowData;
	MouseData mouseData;

	void (*mainLoopCallback)(ApplicationWindow* appWindow) = nullptr;

public:
	ApplicationWindow() = delete;
	ApplicationWindow(int windowW, int windowH, const char* windowName);
	virtual ~ApplicationWindow();

	int MainLoop();
	void ProcessUserInput();
	void Clear() const;

	void SetMainLoopCallback(void (*callback)(ApplicationWindow* appWindow));
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(const Color& c);
	void SetDepthTestEnabled(bool state = true);
	void SetStencilTestEnabled(bool state = true);

	static void SetDepthTestMask(bool mask);
	static void SetDepthTestFunc(unsigned int func);
	static void SetStencilTestMask(unsigned int mask);
	static void SetStencilTestFunc(unsigned int func, int reference, unsigned int mask);
	static void SetStencilTestOp(unsigned int stencilFail, unsigned int depthFail, unsigned int depthPass);

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

	inline float GetAspectRatio() const
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
	int InitOpenGL();
	
	void MouseButtonCallback();
	void MousePositionCallback();

	void UpdateClearColor() const;
};
