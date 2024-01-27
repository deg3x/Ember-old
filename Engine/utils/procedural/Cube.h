#pragma once

#include "Definitions.h"

class Mesh;

class ENGINE_API Cube
{
public:
    Cube() = delete;

    static void GenerateCube(std::shared_ptr<Mesh> targetMesh);
};
