#include "engine_pch.h"
#include "ProceduralMesh.h"

#include "core/components/Mesh.h"

void ProceduralMesh::GenerateQuad(const std::shared_ptr<Mesh>& targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    VertexData data;
    
    data.position = Vector3(-1.0f, 1.0f, 0.0f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.0f, 1.0f);

    vertexData.push_back(data);

    data.position = Vector3(-1.0f, -1.0f, 0.0f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.0f, 0.0f);

    vertexData.push_back(data);

    data.position = Vector3(1.0f, 1.0f, 0.0f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(1.0f, 1.0f);

    vertexData.push_back(data);

    data.position = Vector3(1.0f, -1.0f, 0.0f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(1.0f, 0.0f);

    vertexData.push_back(data);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);

    GenerateTangentsBitangents(vertexData, indices);
    targetMesh->SetMeshData(vertexData, indices);
}

void ProceduralMesh::GenerateCube(const std::shared_ptr<Mesh>& targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    //////// Generate vertex data
    VertexData data;

    // Front
    data.position = Vector3(-0.5f, -0.5f, -0.5f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, -0.5f, -0.5f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.25f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, 0.5f, -0.5f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.0f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, 0.5f, -0.5f);
    data.normal = Vector3(0.0f, 0.0f, -1.0f);
    data.uv = Vector2(0.0f, 0.33f);

    vertexData.push_back(data);

    // Right
    data.position = Vector3(0.5f, -0.5f, -0.5f);
    data.normal = Vector3(1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.25f, 0.33f);

    vertexData.push_back(data);
    
    data.position = Vector3(0.5f, -0.5f, 0.5f);
    data.normal = Vector3(1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, 0.5f, -0.5f);
    data.normal = Vector3(1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.25f, 0.0f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, 0.5f, 0.5f);
    data.normal = Vector3(1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.5f, 0.0f);

    vertexData.push_back(data);

    // Back
    data.position = Vector3(0.5f, -0.5f, 0.5f);
    data.normal = Vector3(0.0f, 0.0f, 1.0f);
    data.uv = Vector2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, -0.5f, 0.5f);
    data.normal = Vector3(0.0f, 0.0f, 1.0f);
    data.uv = Vector2(0.5f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, 0.5f, 0.5f);
    data.normal = Vector3(0.0f, 0.0f, 1.0f);
    data.uv = Vector2(0.75f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, 0.5f, 0.5f);
    data.normal = Vector3(0.0f, 0.0f, 1.0f);
    data.uv = Vector2(0.75f, 0.66f);

    vertexData.push_back(data);

    // Left
    data.position = Vector3(-0.5f, -0.5f, 0.5f);
    data.normal = Vector3(-1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.5f, 0.66f);

    vertexData.push_back(data);
    
    data.position = Vector3(-0.5f, -0.5f, -0.5f);
    data.normal = Vector3(-1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, 0.5f, 0.5f);
    data.normal = Vector3(-1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.5f, 1.0f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, 0.5f, -0.5f);
    data.normal = Vector3(-1.0f, 0.0f, 0.0f);
    data.uv = Vector2(0.25f, 1.0f);

    vertexData.push_back(data);

    // Bottom
    data.position = Vector3(-0.5f, -0.5f, 0.5f);
    data.normal = Vector3(0.0f, -1.0f, 0.0f);
    data.uv = Vector2(0.5f, 0.66f);

    vertexData.push_back(data);
    
    data.position = Vector3(0.5f, -0.5f, 0.5f);
    data.normal = Vector3(0.0f, -1.0f, 0.0f);
    data.uv = Vector2(0.5f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, -0.5f, -0.5f);
    data.normal = Vector3(0.0f, -1.0f, 0.0f);
    data.uv = Vector2(0.25f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, -0.5f, -0.5f);
    data.normal = Vector3(0.0f, -1.0f, 0.0f);
    data.uv = Vector2(0.25f, 0.33f);

    vertexData.push_back(data);

    // Top
    data.position = Vector3(-0.5f, 0.5f, -0.5f);
    data.normal = Vector3(0.0f, 1.0f, 0.0f);
    data.uv = Vector2(1.0f, 0.66f);

    vertexData.push_back(data);
    
    data.position = Vector3(0.5f, 0.5f, -0.5f);
    data.normal = Vector3(0.0f, 1.0f, 0.0f);
    data.uv = Vector2(1.0f, 0.33f);

    vertexData.push_back(data);

    data.position = Vector3(-0.5f, 0.5f, 0.5f);
    data.normal = Vector3(0.0f, 1.0f, 0.0f);
    data.uv = Vector2(0.75f, 0.66f);

    vertexData.push_back(data);

    data.position = Vector3(0.5f, 0.5f, 0.5f);
    data.normal = Vector3(0.0f, 1.0f, 0.0f);
    data.uv = Vector2(0.75f, 0.33f);

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

    GenerateTangentsBitangents(vertexData, indices);
    targetMesh->SetMeshData(vertexData, indices);
}

void ProceduralMesh::GeneratePlane(int resolution, float size, const std::shared_ptr<Mesh>& targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    const float stepSize = size / (float)resolution;
    const float initPosX = -size / 2.0f;
    const float initPosZ = -size / 2.0f;
    
    float currentPosX = initPosX;
    float currentPosZ = initPosZ;

    // Generate vertex data
    for (int i = 0; i <= resolution; i++)
    {
        for (int j = 0; j <= resolution; j++)
        {
            currentPosZ = initPosZ + i * stepSize;
            currentPosX = initPosX + j * stepSize;

            VertexData currentVertex;
            
            currentVertex.position = Vector3(currentPosX, 0.0f, currentPosZ);
            currentVertex.normal = Vector3(0.0f, 1.0f, 0.0f);
            currentVertex.uv = Vector2((float)j / (float)resolution, (float)i / (float)resolution);

            vertexData.push_back(currentVertex);
        }
    }

    // Generate indices
    for (int i = 0; i < resolution; i++)
    {
        unsigned int vertId = i * (resolution + 1);
        unsigned int nextRowVertId = (i+1) * (resolution + 1);
        
        for (int j = 0; j < resolution; j++)
        {
            indices.push_back(vertId);
            indices.push_back(nextRowVertId);
            indices.push_back(vertId + 1);

            indices.push_back(vertId + 1);
            indices.push_back(nextRowVertId);
            indices.push_back(nextRowVertId + 1);

            vertId++;
            nextRowVertId++;
        }
    }

    GenerateTangentsBitangents(vertexData, indices);
    targetMesh->SetMeshData(vertexData, indices);
}

void ProceduralMesh::GenerateSphere(int sectors, int stacks, float radius, const std::shared_ptr<Mesh>& targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    Vector3 vertPos;
    const float stackStep  = PI / static_cast<float>(stacks);
    const float sectorStep = TWO_PI / static_cast<float>(sectors);

    // Generate vertex data
    for (int i = 0; i <= stacks; i++)
    {
        const float stackAngle = HALF_PI - (i * stackStep);
        const float sectorRadius = radius * Cos(stackAngle);
        vertPos.z = radius * Sin(stackAngle);

        for (int j = 0; j <= sectors; j++)
        {
            vertPos.x = sectorRadius * Cos(j * sectorStep);
            vertPos.y = sectorRadius * Sin(j * sectorStep);

            VertexData currentVertex;

            currentVertex.position = Vector3(vertPos.x, vertPos.y, vertPos.z);
            currentVertex.normal = Vector3::Normalize(currentVertex.position);
            currentVertex.uv = Vector2(static_cast<float>(j) / static_cast<float>(sectors), static_cast<float>(i) / static_cast<float>(stacks));

            vertexData.push_back(currentVertex);
        }
    }

    // Generate indices
    for (int i = 0; i < stacks; i++)
    {
        unsigned int sid = i * (sectors + 1);
        unsigned int nsid = (i + 1) * (sectors + 1);

        for (int j = 0; j < sectors; j++)
        {
            if (i != 0)
            {
                indices.push_back(sid);
                indices.push_back(nsid);
                indices.push_back(sid + 1);
            }

            if (i != stacks - 1)
            {
                indices.push_back(sid + 1);
                indices.push_back(nsid);
                indices.push_back(nsid + 1);
            }

            sid++;
            nsid++;
        }
    }

    GenerateTangentsBitangents(vertexData, indices);
    targetMesh->SetMeshData(vertexData, indices);
}

void ProceduralMesh::GenerateTangentsBitangents(std::vector<VertexData>& vertexData, const std::vector<unsigned int>& indices)
{
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        VertexData& vert1 = vertexData[indices[i]];
        VertexData& vert2 = vertexData[indices[i + 1]];
        VertexData& vert3 = vertexData[indices[i + 2]];
        
        const Vector3 dPos1 = vert2.position - vert1.position;
        const Vector3 dPos2 = vert3.position - vert1.position;
        const Vector2 dUV1  = vert2.uv - vert1.uv;
        const Vector2 dUV2  = vert3.uv - vert1.uv;

        const float invDet = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

        Vector3 tangent = invDet * (dUV2.y * dPos1 - dUV1.y * dPos2);
        Vector3 bitangent = invDet * (dUV1.x * dPos2 - dUV2.x * dPos1);

        tangent   = tangent.Normalize();
        bitangent = bitangent.Normalize();

        vert1.tangent   = tangent;
        vert1.bitangent = bitangent;
        vert2.tangent   = tangent;
        vert2.bitangent = bitangent;
        vert3.tangent   = tangent;
        vert3.bitangent = bitangent;
    }
}