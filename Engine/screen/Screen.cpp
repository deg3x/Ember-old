#include "Screen.h"

#include <string>
#include <glfw/glfw3.h>

std::string Screen::GetScreenName()
{
    return glfwGetMonitorName(glfwGetPrimaryMonitor());
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
