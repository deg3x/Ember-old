#pragma once

#include "Definitions.h"

class ENGINE_API Time
{
public:
    static float TimeSinceStartup;
    static float DeltaTime;

private:
    static float lastFrameTimestamp;

public:
    static void Initialize();
    static void Tick();
};
