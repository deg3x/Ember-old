#pragma once
#include <vector>
#include "Types.h"

class Plane
{
private:
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

	int resolution;
	float size;

public:
	Plane();
	Plane(int initResolution, float initSize);
	virtual ~Plane();

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