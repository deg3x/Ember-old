#pragma once

#include "Definitions.h"

class ENGINE_API Time
{
public:
    static float TimeSinceStartup;
    static float DeltaTime;

public:
    static void Initialize();
    static void Tick();
};
