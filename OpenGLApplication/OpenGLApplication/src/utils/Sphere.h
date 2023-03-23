#pragma once
#include <vector>
#include "Types.h"
#include "Mesh.h"

class Sphere : public Mesh
{
private:
	int sectors;
	int stacks;
	float radius;

public:
	Sphere();
	Sphere(int initSectors, int initStacks, float initRadius);
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