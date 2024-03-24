#include "engine_pch.h"
#include "Window.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "logger/Logger.h"
#include "screen/Screen.h"

GLFWwindow* Window::window;
WindowData Window::windowData;

void Window::Initialize()
{
    if (!glfwInit())
    {
        Logger::Log(LogCategory::ERROR, "Failed to initialize GLFW", "Window::Initialize");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);
#endif

    VideoMode mode = Screen::GetCurrentVideoMode();
    
    windowData.title       = "Ember Engine";
    windowData.windowW     = 1366;
    windowData.windowH     = 768;
    windowData.aspectRatio = 1366.0f / 768.0f;

    window = glfwCreateWindow(windowData.windowW, windowData.windowH, windowData.title, NULL, NULL);
    if (window == NULL)
    {
        Logger::Log(LogCategory::ERROR, "Failed to create GLFW window", "Window::Initialize");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    SetVSync(false);

    Logger::Log(LogCategory::INFO, "Window initialization completed successfully", "Window::Initialize");
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::SetVSync(bool state)
{
    glfwSwapInterval(state ? 1 : 0);

    windowData.vsync = state;
}
