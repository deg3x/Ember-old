#pragma once

#include <vector>

class Sphere
{
private:
	std::vector<float> vertexData;
	std::vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	int sectors;
	int stacks;
	float radius;

public:
	Sphere();
	Sphere(int sectors, int stacks, float radius);
	~Sphere();

	void Init();
	void Draw(bool wireframe);
	void CreateVertices();
	void CreateIndices();

	inline unsigned int GetVAO()
	{
		return VAO;
	}

	inline unsigned int GetVBO()
	{
		return VBO;
	}

	inline unsigned int GetEBO()
	{
		return EBO;
	}

	inline unsigned int GetIndicesSize()
	{
		return indices.size();
	}
		
};