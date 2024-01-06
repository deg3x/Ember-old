#pragma once

#include "Definitions.h"

class ENGINE_API Logger
{
public:
    static void LogInfo(const char* message, const char* context = "NO CONTEXT");
    static void LogWarning(const char* message, const char* context = "NO CONTEXT");
    static void LogError(const char* message, const char* context = "NO CONTEXT");
};
