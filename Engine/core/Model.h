#pragma once

#include "Definitions.h"

class Mesh;

class ENGINE_API Model
{
public:
    Model() = delete;
    
    static std::vector<std::shared_ptr<Mesh>> Load(const char* path);
};