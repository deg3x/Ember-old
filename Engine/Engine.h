#pragma once

#include "Definitions.h"

class ENGINE_API Engine
{
public:
    Engine() = delete;

    static void Initialize();
    static void Tick();
};
