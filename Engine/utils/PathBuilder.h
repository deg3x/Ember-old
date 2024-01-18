#pragma once

#include "Definitions.h"

#include <string>

class ENGINE_API PathBuilder
{
public:
    static std::string GetPath(const std::string& path);
};
