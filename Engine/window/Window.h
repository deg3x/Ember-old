#pragma once

#include "Definitions.h"

struct GLFWimage;

struct WindowData
{
    GLFWimage* icon;
    const char* title;
    int windowW;
    int windowH;
    float aspectRatio;
    bool vsync;
};

struct GLFWwindow;

class ENGINE_API Window
{
private:
    // Leave this as a raw pointer since we only manage it internally
    static GLFWwindow* window;
    static WindowData windowData;

public:
    Window() = delete;

    static void Initialize();
    static void SwapBuffers();
    static bool ShouldClose();
    static void SetVSync(bool state);

    inline static bool GetVSync()
    {
        return windowData.vsync;
    }

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
};
