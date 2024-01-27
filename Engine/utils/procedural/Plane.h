#pragma once

#include "Definitions.h"

class Mesh;

class ENGINE_API Plane
{
public:
    Plane() = delete;
    
    static void GeneratePlane(int resolution, float size, std::shared_ptr<Mesh> targetMesh);
};
