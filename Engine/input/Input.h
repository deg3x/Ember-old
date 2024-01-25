#pragma once

#include "Definitions.h"
#include "KeyCodes.h"
#include "Mouse.h"

class ENGINE_API Input
{
public:
    static MouseData Mouse;
    
public:
    Input() = delete;

    static void Initialize();
    static void Tick();
    
    static bool GetKey(int keycode);
    
    static bool GetMouse(int mouseBtn);
    static bool GetMouseClick(int mouseBtn);
    static bool GetMouseDrag(int mouseBtn);
    static glm::dvec2 GetMousePos();
    
protected:
    static void UpdateMouseData();
};
