#pragma once

#include "Definitions.h"

class Mesh;
struct VertexData;

class ENGINE_API ProceduralMesh
{
public:
    ProceduralMesh() = delete;

    static void GenerateQuad(const std::shared_ptr<Mesh>& targetMesh);
    static void GenerateCube(const std::shared_ptr<Mesh>& targetMesh);
    static void GeneratePlane(int resolution, float size, const std::shared_ptr<Mesh>& targetMesh);
    static void GenerateSphere(int sectors, int stacks, float radius, const std::shared_ptr<Mesh>& targetMesh);

    static void GenerateTangentsBitangents(std::vector<VertexData>& vertexData, const std::vector<unsigned int>& indices);
};
