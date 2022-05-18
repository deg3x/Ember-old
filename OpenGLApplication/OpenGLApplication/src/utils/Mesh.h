#pragma once
#include <vector>
#include "Shader.h"
#include "../utils/Types.h"

class Mesh
{
private:
	// Add support for textures later
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void SetupMesh();

public:
	Mesh() = delete;
	Mesh(std::vector<VertexData> data, std::vector<unsigned int> indices);
	virtual ~Mesh();

	void DrawMesh() const;
};