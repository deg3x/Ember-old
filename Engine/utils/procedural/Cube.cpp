#include "engine_pch.h"
#include "Cube.h"

#include "core/components/Mesh.h"

void Cube::GenerateCube(std::shared_ptr<Mesh> targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    //////// Generate vertex data
    VertexData data;

    // Front
    data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.25f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.0f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    data.uv = glm::vec2(0.0f, 0.33f);

    vertexData.push_back(data);

    // Right
    data.position = glm::vec3(0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 0.33f);

    vertexData.push_back(data);
    
    data.position = glm::vec3(0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 0.0f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 0.0f);

    vertexData.push_back(data);

    // Back
    data.position = glm::vec3(0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    data.uv = glm::vec2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    data.uv = glm::vec2(0.5f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    data.uv = glm::vec2(0.75f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    data.uv = glm::vec2(0.75f, 0.66f);

    vertexData.push_back(data);

    // Left
    data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 0.66f);

    vertexData.push_back(data);
    
    data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 1.0f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 1.0f);

    vertexData.push_back(data);

    // Bottom
    data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 0.66f);

    vertexData.push_back(data);
    
    data.position = glm::vec3(0.5f, -0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    data.uv = glm::vec2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, -0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    data.uv = glm::vec2(0.25f, 0.33f);

    vertexData.push_back(data);

    // Top
    data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    data.uv = glm::vec2(1.0f, 0.66f);

    vertexData.push_back(data);
    
    data.position = glm::vec3(0.5f, 0.5f, -0.5f);
    data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    data.uv = glm::vec2(1.0f, 0.33f);

    vertexData.push_back(data);

    data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    data.uv = glm::vec2(0.75f, 0.66f);

    vertexData.push_back(data);

    data.position = glm::vec3(0.5f, 0.5f, 0.5f);
    data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    data.uv = glm::vec2(0.75f, 0.33f);

    vertexData.push_back(data);
    ////////

    // Generate indices
    for (int i = 0; i < 6; i++)
    {
        indices.push_back(0 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(3 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
    }

    targetMesh->SetMeshData(vertexData, indices);
}
