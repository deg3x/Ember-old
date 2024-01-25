#pragma once

#include "Definitions.h"
#include "ScreenTypes.h"

class ENGINE_API Screen
{
public:
    Screen() = delete;

    static std::string GetScreenName();
    static VideoMode GetCurrentVideoMode();
    static std::vector<VideoMode> GetAvailableVideoModes();
};
