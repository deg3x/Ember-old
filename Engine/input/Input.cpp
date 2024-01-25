#include "engine_pch.h"
#include "Input.h"

#include "window/Window.h"
#include "logger/Logger.h"

#include "glfw/glfw3.h"
#include "glm/gtc/epsilon.hpp"

MouseData Input::Mouse;

void Input::Initialize()
{
    glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    Mouse.sensitivity = 0.01f;

    Logger::Log(LogCategory::INFO, "Input system initialization completed successfully", "Input::Initialize");
}

void Input::Tick()
{
    UpdateMouseData();
    
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

glm::dvec2 Input::GetMousePos()
{
    glm::dvec2 mousePos;

    glfwGetCursorPos(Window::GetWindow(), &mousePos.x, &mousePos.y);

    return mousePos;
}

void Input::UpdateMouseData()
{
    Mouse.leftButtonPressedLastFrame   = Mouse.leftButtonPressed;
    Mouse.rightButtonPressedLastFrame  = Mouse.rightButtonPressed;
    Mouse.middleButtonPressedLastFrame = Mouse.middleButtonPressed;
        
    Mouse.leftButtonPressed   = GetMouse(MOUSE_BTN_LEFT);
    Mouse.rightButtonPressed  = GetMouse(MOUSE_BTN_RIGHT);
    Mouse.middleButtonPressed = GetMouse(MOUSE_BTN_MIDDLE);

    glm::dvec2 mousePos;
    glfwGetCursorPos(Window::GetWindow(), &mousePos.x, &mousePos.y);

    Mouse.leftMouseDragDeltaX  = mousePos.x - Mouse.posX;
    Mouse.leftMouseDragDeltaY  = mousePos.y - Mouse.posY;
    Mouse.rightMouseDragDeltaX = mousePos.x - Mouse.posX;
    Mouse.rightMouseDragDeltaY = mousePos.y - Mouse.posY;
    
    Mouse.posX = mousePos.x;
    Mouse.posY = mousePos.y;
}
