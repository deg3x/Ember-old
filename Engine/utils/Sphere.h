#pragma once
#include "../core/Mesh.h"

class Sphere : public Mesh
{
private:
	int sectors;
	int stacks;
	float radius;

public:
	Sphere();
	Sphere(int initSectors, int initStacks, float initRadius);
	Sphere(int initSectors, int initStacks, float initRadius, const std::shared_ptr<Material>& initMaterial);
	Sphere(int initSectors, int initStacks, float initRadius, const char* vertShader, const char* fragShader);
	virtual ~Sphere() override = default;

	void GenerateVertexData();
	void GenerateIndices();
};