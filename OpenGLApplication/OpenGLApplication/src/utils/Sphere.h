#pragma once

#include <vector>
#include "Types.h"

class Sphere
{
private:
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

	int sectors;
	int stacks;
	float radius;

public:
	Sphere();
	Sphere(int sectors, int stacks, float radius);
	~Sphere();

	void GenerateVertexData();
	void GenerateIndices();

	inline std::vector<VertexData> GetVertexData()
	{
		return this->vertexData;
	}

	inline std::vector<unsigned int> GetIndices()
	{
		return this->indices;
	}
};