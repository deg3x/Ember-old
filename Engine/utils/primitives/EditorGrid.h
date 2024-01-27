#pragma once

#include "Definitions.h"

class Object;

class EditorGrid
{
public:
    EditorGrid() = delete;

    static std::shared_ptr<Object> Instantiate();
};
