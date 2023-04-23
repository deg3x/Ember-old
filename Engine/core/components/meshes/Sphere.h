#pragma once

#include "Mesh.h"

class Sphere : public Mesh
{
private:
	int sectors;
	int stacks;
	float radius;

public:
	Sphere(const std::shared_ptr<Material>& initMaterial);
	Sphere(int initSectors, int initStacks, float initRadius, const std::shared_ptr<Material>& initMaterial);
	virtual ~Sphere() override = default;

	void GenerateVertexData();
	void GenerateIndices();
};