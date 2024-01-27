#pragma once

#include "Definitions.h"

class Object;

class Skybox
{
public:
    Skybox() = delete;

    static std::shared_ptr<Object> Instantiate();
};
