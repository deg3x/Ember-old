#pragma once
#include <vector>
#include "Types.h"
#include "../core/Mesh.h"

class Plane : public Mesh
{
private:
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
		return vertexData;
	}

	inline std::vector<unsigned int> GetIndices()
	{
		return indices;
	}
};