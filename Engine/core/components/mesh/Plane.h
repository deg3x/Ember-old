#pragma once
#include "Mesh.h"

class Plane : public Mesh
{
private:
	int resolution;
	float size;

public:
	Plane();
	Plane(int initResolution, float initSize);
	Plane(int initResolution, float initSize, const std::shared_ptr<Material>& initMaterial);
	Plane(int initResolution, float initSize, const char* vertShader, const char* fragShader);
	virtual ~Plane() override = default;

	void GenerateVertexData();
	void GenerateIndices();
};