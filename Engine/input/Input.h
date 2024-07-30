#pragma once

#include "Definitions.h"
#include "Mouse.h"

struct GLFWwindow;

class ENGINE_API Input
{
public:
    static MouseData Mouse;

private:
    inline static constexpr float scrollResetTimer = 0.2f;
    inline static float currentScrollResetTimer;
    
public:
    Input() = delete;

    static void Initialize();
    static void Tick();
    
    static bool GetKey(int keycode);
    
    static bool GetMouse(int mouseBtn);
    static bool GetMouseClick(int mouseBtn);
    static bool GetMouseDrag(int mouseBtn);
    static Vector2 GetMousePos();
    
protected:
    static void UpdateMouseData();
    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};
