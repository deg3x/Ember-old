#include "engine_pch.h"
#include "Time.h"

#include "glfw/glfw3.h"

float Time::DeltaTime;
double Time::TimeSinceStartup;

void Time::Initialize()
{
    DeltaTime        = 0.0f;
    TimeSinceStartup = 0.0;
}

void Time::Tick()
{
    const double currentTime = glfwGetTime();
    
    DeltaTime        = static_cast<float>(currentTime - TimeSinceStartup);
    TimeSinceStartup = currentTime;
}
