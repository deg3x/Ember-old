#include "Time.h"

#include <glfw/glfw3.h>

float Time::DeltaTime;
float Time::TimeSinceStartup;

void Time::Initialize()
{
    DeltaTime          = 0.0f;
    TimeSinceStartup   = 0.0f;
}

void Time::Tick()
{
    const float currentTime = static_cast<float>(glfwGetTime());
    
    DeltaTime        = currentTime - TimeSinceStartup;
    TimeSinceStartup = currentTime;
}
