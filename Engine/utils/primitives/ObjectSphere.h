#pragma once

#include "Definitions.h"

class Object;

class ENGINE_API ObjectSphere
{
public:
    ObjectSphere() = delete;

    static std::shared_ptr<Object> Instantiate();
};
