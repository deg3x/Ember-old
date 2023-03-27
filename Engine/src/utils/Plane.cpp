#include "Plane.h"

Plane::Plane()
{
	resolution = 10;
	size = 1.0f;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Plane::Plane(int initResolution, float initSize)
{
	resolution = initResolution < 1 ? 1 : initResolution;
	size = initSize < 1.0f ? 1.0f : initSize;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Plane::~Plane()
{

}

void Plane::GenerateVertexData()
{
	std::vector<VertexData>().swap(vertexData);

	float stepSize = size / (float)resolution;
	float initPosX = -size / 2.0f;
	float initPosZ = -size / 2.0f;
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

			vertexData.push_back(currentVertex);
		}
	}
}

void Plane::GenerateIndices()
{
	std::vector<unsigned int>().swap(indices);

	unsigned int vertId;
	unsigned int nextRowVertId;

	for (int i = 0; i < resolution; i++)
	{
		vertId = i * (resolution + 1);
		nextRowVertId = (i+1) * (resolution + 1);
		
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