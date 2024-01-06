#include "Quad.h"

Quad::Quad(const std::shared_ptr<Material> &initMaterial) : Mesh(initMaterial)
{
    GenerateVertexData();
    GenerateIndices();
    SetupMesh();
}

void Quad::GenerateVertexData()
{
    VertexData data;
    
    data.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.0f, 0.0f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, -0.5f, 0.0f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(1.0f, 0.0f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, 0.0f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.0f, 1.0f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, 0.0f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(1.0f, 1.0f);

    vertexData.push_back(data);
}

void Quad::GenerateIndices()
{
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
}

