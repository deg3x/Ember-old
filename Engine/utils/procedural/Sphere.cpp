#include "engine_pch.h"
#include "Sphere.h"

#include "core/components/Mesh.h"

void Sphere::GenerateSphere(int sectors, int stacks, float radius, std::shared_ptr<Mesh> targetMesh)
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
}
