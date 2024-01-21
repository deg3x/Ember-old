#include "Time.h"

#include <glfw/glfw3.h>

float Time::DeltaTime;
float Time::TimeSinceStartup;
float Time::lastFrameTimestamp;

void Time::Initialize()
{
    DeltaTime          = 0.0f;
    TimeSinceStartup   = 0.0f;
    lastFrameTimestamp = 0.0f;
}

void Time::Tick()
{
    const float currentTime = static_cast<float>(glfwGetTime());
    
    DeltaTime          = currentTime - lastFrameTimestamp;
    lastFrameTimestamp = currentTime;
    TimeSinceStartup  += DeltaTime;
}
