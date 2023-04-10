#pragma once

#include "../utils/Types.h"
#include "Component.h"

#include <vector>

class Mesh : public Component
{
protected:
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

protected:
	void SetupMesh();

public:
	Mesh() = default;
	Mesh(std::vector<VertexData> data, std::vector<unsigned int> initIndices);
	virtual ~Mesh();

	void DrawMesh() const;
};