#include "Plane.h"

Plane::Plane()
{
	this->resolution = 10;
	this->size = 1.0f;

	this->GenerateVertexData();
	this->GenerateIndices();
}

Plane::Plane(int initResolution, float initSize)
{
	this->resolution = initResolution < 1 ? 1 : initResolution;
	this->size = initSize < 1.0f ? 1.0f : initSize;

	this->GenerateVertexData();
	this->GenerateIndices();
}

Plane::~Plane()
{

}

void Plane::GenerateVertexData()
{
	std::vector<VertexData>().swap(vertexData);

	float stepSize = this->size / (float)this->resolution;
	float initPosX = -this->size / 2.0f;
	float initPosZ = -this->size / 2.0f;
	float currentPosX = initPosX;
	float currentPosZ = initPosZ;

	for (int i = 0; i <= this->resolution; i++)
	{
		for (int j = 0; j <= this->resolution; j++)
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

	for (int i = 0; i < this->resolution; i++)
	{
		vertId = i * (this->resolution + 1);
		nextRowVertId = (i+1) * (this->resolution + 1);
		
		for (int j = 0; j < this->resolution; j++)
		{
			this->indices.push_back(vertId);
			this->indices.push_back(nextRowVertId);
			this->indices.push_back(vertId + 1);

			this->indices.push_back(vertId + 1);
			this->indices.push_back(nextRowVertId);
			this->indices.push_back(nextRowVertId + 1);

			vertId++;
			nextRowVertId++;
		}
	}
}