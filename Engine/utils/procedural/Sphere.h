#pragma once

#include "Definitions.h"

class Mesh;

class ENGINE_API Sphere
{
public:
    Sphere() = delete;

    static void GenerateSphere(int sectors, int stacks, float radius, std::shared_ptr<Mesh> targetMesh);
};
