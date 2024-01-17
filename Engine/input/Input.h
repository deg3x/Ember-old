#pragma once

#include <glm/vec2.hpp>

#include "Definitions.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

class ENGINE_API Input
{
public:
    Input() = delete;

    static void Initialize();
    static void PollInput();
    
    static bool GetKey(int keycode);
    
    static bool GetMouse(int mouseBtn);
    static glm::vec2<double> GetMousePos();
};
