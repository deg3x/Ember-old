#pragma once

#include "Definitions.h"

class ENGINE_API Time
{
public:
    static float DeltaTime;
    static double TimeSinceStartup;

public:
    static void Initialize();
    static void Tick();
};
