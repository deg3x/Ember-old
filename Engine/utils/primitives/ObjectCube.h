#pragma once

#include "Definitions.h"

class Object;

class ENGINE_API ObjectCube
{
public:
    ObjectCube() = delete;

    static std::shared_ptr<Object> Instantiate();
};
