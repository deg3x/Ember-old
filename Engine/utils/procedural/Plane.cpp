#include "engine_pch.h"
#include "Plane.h"

#include "core/components/Mesh.h"

void Plane::GeneratePlane(int resolution, float size, std::shared_ptr<Mesh> targetMesh)
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
}