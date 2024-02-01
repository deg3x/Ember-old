#pragma once

#include "Definitions.h"

class Object;

class ENGINE_API ObjectPlane
{
public:
    ObjectPlane() = delete;

    static std::shared_ptr<Object> Instantiate();
};
