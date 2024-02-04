#include "engine_pch.h"
#include "ProceduralMesh.h"

#include "core/components/Mesh.h"

void ProceduralMesh::GenerateCube(const std::shared_ptr<Mesh>& targetMesh)
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
    GenerateTangentsBitangents(targetMesh);
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
			
            currentVertex.position = glm::vec3(currentPosX, 0.0f, currentPosZ);
            currentVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            currentVertex.uv = glm::vec2((float)i / (float)resolution, (float)j / (float)resolution);

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

    targetMesh->SetMeshData(vertexData, indices);
    GenerateTangentsBitangents(targetMesh);
}

void ProceduralMesh::GenerateSphere(int sectors, int stacks, float radius, const std::shared_ptr<Mesh>& targetMesh)
{
    std::vector<VertexData> vertexData;
    std::vector<unsigned int> indices;

    glm::vec3 vertPos;
    const float stackStep  = glm::pi<float>() / static_cast<float>(stacks);
    const float sectorStep = 2.0f * glm::pi<float>() / static_cast<float>(sectors);

    // Generate vertex data
    for (int i = 0; i <= stacks; i++)
    {
        const float stackAngle = (glm::pi<float>() * 0.5f) - (i * stackStep);
        const float sectorRadius = radius * glm::cos(stackAngle);
        vertPos.z = radius * glm::sin(stackAngle);

        for (int j = 0; j <= sectors; j++)
        {
            vertPos.x = sectorRadius * glm::cos(j * sectorStep);
            vertPos.y = sectorRadius * glm::sin(j * sectorStep);

            VertexData currentVertex;

            currentVertex.position = glm::vec3(vertPos.x, vertPos.y, vertPos.z);
            currentVertex.normal = glm::normalize(currentVertex.position);
            currentVertex.uv = glm::vec2(static_cast<float>(j) / static_cast<float>(sectors), static_cast<float>(i) / static_cast<float>(stacks));

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

    targetMesh->SetMeshData(vertexData, indices);
    GenerateTangentsBitangents(targetMesh);
}

void ProceduralMesh::GenerateTangentsBitangents(const std::shared_ptr<Mesh>& targetMesh)
{
    for (size_t i = 0; i < targetMesh->indices.size(); i += 3)
    {
        VertexData& vert1 = targetMesh->vertexData[targetMesh->indices[i]];
        VertexData& vert2 = targetMesh->vertexData[targetMesh->indices[i + 1]];
        VertexData& vert3 = targetMesh->vertexData[targetMesh->indices[i + 2]];
        
        const glm::vec3 dPos1 = vert2.position - vert1.position;
        const glm::vec3 dPos2 = vert3.position - vert1.position;
        const glm::vec2 dUV1  = vert2.uv - vert1.uv;
        const glm::vec2 dUV2  = vert3.uv - vert1.uv;

        const float invDet = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

        glm::vec3 tangent = invDet * (dUV2.y * dPos1 - dUV1.y * dPos2);
        glm::vec3 bitangent = invDet * (dUV1.x * dPos2 - dUV2.x * dPos1);

        tangent   = glm::normalize(tangent);
        bitangent = glm::normalize(bitangent);

        vert1.tangent   = tangent;
        vert1.bitangent = bitangent;
        vert2.tangent   = tangent;
        vert2.bitangent = bitangent;
        vert3.tangent   = tangent;
        vert3.bitangent = bitangent;
    }
}