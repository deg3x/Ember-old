#include "Screen.h"

#include <string>
#include <glfw/glfw3.h>

std::string Screen::GetScreenName()
{
    return glfwGetMonitorName(glfwGetPrimaryMonitor());
}

VideoMode Screen::GetCurrentVideoMode()
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    VideoMode ret = {};

    ret.width     = mode->width;
    ret.height    = mode->height;
    ret.framerate = mode->refreshRate;

    return ret;
}

std::vector<VideoMode> Screen::GetAvailableVideoModes()
{
    int count;
    const GLFWvidmode* mode = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

    std::vector<VideoMode> ret;

    for (int i = 0; i < count; i++)
    {
        VideoMode currentMode = {};
        
        currentMode.width     = mode[i].width;
        currentMode.height    = mode[i].height;
        currentMode.framerate = mode[i].refreshRate;

        ret.emplace_back(currentMode);
    }

    return ret;
}
