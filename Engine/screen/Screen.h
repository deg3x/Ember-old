#pragma once
#include "Definitions.h"
#include <string>
#include <vector>

struct VideoMode
{
    int width;
    int height;
    int framerate;
};

class ENGINE_API Screen
{
public:
    Screen() = delete;

    static std::string GetScreenName();
    static VideoMode GetCurrentVideoMode();
    static std::vector<VideoMode> GetAvailableVideoModes();
};
