#include "Plane.h"

#include "../../materials/Material.h"

Plane::Plane(const std::shared_ptr<Material>& initMaterial) : Mesh(initMaterial)
{
	resolution = 10;
	size = 1.0f;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Plane::Plane(int initResolution, float initSize, const std::shared_ptr<Material>& initMaterial) : Mesh(initMaterial)
{
	resolution = initResolution < 1 ? 1 : initResolution;
	size = initSize < 1.0f ? 1.0f : initSize;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

void Plane::GenerateVertexData()
{
	std::vector<VertexData>().swap(vertexData);

	const float stepSize = size / (float)resolution;
	const float initPosX = -size / 2.0f;
	const float initPosZ = -size / 2.0f;
	
	float currentPosX = initPosX;
	float currentPosZ = initPosZ;

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
}

void Plane::GenerateIndices()
{
	std::vector<unsigned int>().swap(indices);

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
}