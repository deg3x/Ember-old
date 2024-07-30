#include "engine_pch.h"
#include "Input.h"

#include "core/Time.h"
#include "window/Window.h"
#include "logger/Logger.h"

#include "glfw/glfw3.h"

MouseData Input::Mouse;

void Input::Initialize()
{
    glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    Mouse.sensitivity       = 1.0f;
    Mouse.sensitivityScroll = 10.0f;
    
    currentScrollResetTimer = scrollResetTimer;

    glfwSetScrollCallback(Window::GetWindow(), ScrollCallback);

    Logger::Log(LogCategory::INFO, "Input system initialization completed successfully", "Input::Initialize");
}

void Input::Tick()
{
    UpdateMouseData();

    if (currentScrollResetTimer > 0.0f)
    {
        currentScrollResetTimer -= Time::DeltaTime;
    }
    
    // We should move this call to our own Event queue Poll function
    glfwPollEvents();
}

bool Input::GetKey(int keycode)
{
    const int state = glfwGetKey(Window::GetWindow(), keycode);

    // GLFW_REPEAT only returns in the key callback, per GLFW documentation,
    // so this may be redundant
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::GetMouse(int mouseBtn)
{
    const int state = glfwGetMouseButton(Window::GetWindow(), mouseBtn);

    return state == GLFW_PRESS;
}

bool Input::GetMouseClick(int mouseBtn)
{
    switch (mouseBtn)
    {
    case MOUSE_BTN_LEFT:
        return !Mouse.leftButtonPressed && Mouse.leftButtonPressedLastFrame;
    case MOUSE_BTN_RIGHT:
        return !Mouse.rightButtonPressed && Mouse.rightButtonPressedLastFrame;
    default:
        Logger::Log(LogCategory::WARNING, "Mouse click queried for unsupported button...", "Input::GetMouseClick");
        return false;
    }
}

bool Input::GetMouseDrag(int mouseBtn)
{
    switch (mouseBtn)
    {
    case MOUSE_BTN_LEFT:
        return Mouse.leftButtonPressed && Mouse.leftButtonPressedLastFrame;
    case MOUSE_BTN_RIGHT:
        return Mouse.rightButtonPressed && Mouse.rightButtonPressedLastFrame;
    default:
        Logger::Log(LogCategory::WARNING, "Mouse drag queried for unsupported button...", "Input::GetMouseDrag");
        return false;
    }
}

Vector2 Input::GetMousePos()
{
    double mousePosX;
    double mousePosY;

    glfwGetCursorPos(Window::GetWindow(), &mousePosX, &mousePosY);

    return Vector2(static_cast<real>(mousePosX), static_cast<real>(mousePosY));
}

void Input::UpdateMouseData()
{
    Mouse.leftButtonPressedLastFrame   = Mouse.leftButtonPressed;
    Mouse.rightButtonPressedLastFrame  = Mouse.rightButtonPressed;
    Mouse.middleButtonPressedLastFrame = Mouse.middleButtonPressed;
        
    Mouse.leftButtonPressed   = GetMouse(MOUSE_BTN_LEFT);
    Mouse.rightButtonPressed  = GetMouse(MOUSE_BTN_RIGHT);
    Mouse.middleButtonPressed = GetMouse(MOUSE_BTN_MIDDLE);

    double mousePosX;
    double mousePosY;
    glfwGetCursorPos(Window::GetWindow(), &mousePosX, &mousePosY);

    Mouse.posDeltaX  = mousePosX - Mouse.posX;
    Mouse.posDeltaY  = mousePosY - Mouse.posY;
    
    Mouse.posX = mousePosX;
    Mouse.posY = mousePosY;

    // Reset scroll delta. Will be updated in glfwPollEvents() that gets called after UpdateMouseData().
    float smoothing = Saturate(currentScrollResetTimer / scrollResetTimer);
    smoothing *= smoothing;
    
    Mouse.scrollDeltaX       = 0.0f;
    Mouse.scrollDeltaY       = 0.0f;
    Mouse.scrollDeltaXSmooth = smoothing * Mouse.scrollDeltaLastX;
    Mouse.scrollDeltaYSmooth = smoothing * Mouse.scrollDeltaLastY;
}

void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Mouse.scrollDeltaX       = xOffset;
    Mouse.scrollDeltaY       = yOffset;
    Mouse.scrollDeltaXSmooth = xOffset;
    Mouse.scrollDeltaYSmooth = yOffset;
    Mouse.scrollDeltaLastX   = xOffset;
    Mouse.scrollDeltaLastY   = yOffset;

    currentScrollResetTimer = scrollResetTimer;
}
