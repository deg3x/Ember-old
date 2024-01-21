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

void Input::PollInput()
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

glm::dvec2 Input::GetMousePos()
{
    glm::dvec2 mousePos;

    glfwGetCursorPos(Window::GetWindow(), &mousePos.x, &mousePos.y);

    return mousePos;
}

void Input::ResetMouseOffsetData()
{
    Mouse.leftMouseDragDeltaX  = 0.0;
    Mouse.leftMouseDragDeltaY  = 0.0;
    Mouse.rightMouseDragDeltaX = 0.0;
    Mouse.rightMouseDragDeltaY = 0.0;
}

void Input::UpdateMouseData()
{
    Mouse.leftButtonPressed   = GetMouse(MOUSE_BTN_LEFT);
    Mouse.rightButtonPressed  = GetMouse(MOUSE_BTN_RIGHT);
    Mouse.middleButtonPressed = GetMouse(MOUSE_BTN_MIDDLE);

    glm::dvec2 mousePos;

    glfwGetCursorPos(Window::GetWindow(), &mousePos.x, &mousePos.y);
	
    if (glm::epsilonEqual(mousePos.x, Mouse.posX, DOUBLE_SMALL) && glm::epsilonEqual(mousePos.y, Mouse.posY, DOUBLE_SMALL))
    {
        return;
    }

    Mouse.leftMouseDragDeltaX = Mouse.leftButtonPressed ? mousePos.x - Mouse.posX : 0.0;
    Mouse.leftMouseDragDeltaY = Mouse.leftButtonPressed ? mousePos.y - Mouse.posY : 0.0;

    Mouse.rightMouseDragDeltaX = Mouse.rightButtonPressed ? mousePos.x - Mouse.posX : 0.0;
    Mouse.rightMouseDragDeltaY = Mouse.rightButtonPressed ? mousePos.y - Mouse.posY : 0.0;
    
    Mouse.posX = mousePos.x;
    Mouse.posY = mousePos.y;
}
