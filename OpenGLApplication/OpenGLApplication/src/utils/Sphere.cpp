#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.h"

#include <vector>

Sphere::Sphere()
{
	this->sectors = 10;
	this->stacks = 10;
	this->radius = 0.5f;

	CreateVertices();
	CreateIndices();
}

Sphere::Sphere(int sectors, int stacks, float radius)
{
	this->sectors = sectors < 4 ? 4 : sectors;
	this->stacks = stacks < 3 ? 3 : stacks;
	this->radius = radius;

	CreateVertices();
	CreateIndices();
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Sphere::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Sphere::Draw(bool wireframe)
{
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Sphere::CreateVertices()
{
	std::vector<float>().swap(vertexData);

	float x;
	float y;
	float z;
	float stackAngle;
	float sectorRadius;

	float stackStep = glm::pi<float>() / (float)stacks;
	float sectorStep = 2.0f * glm::pi<float>() / (float)sectors;

	for (int i = 0; i <= stacks; i++)
	{
		stackAngle = (glm::pi<float>() * 0.5f) - (i * stackStep);
		sectorRadius = radius * glm::cos(stackAngle);
		z = radius * glm::sin(stackAngle);

		for (int j = 0; j <= sectors; j++)
		{
			x = sectorRadius * glm::cos(j * sectorStep);
			y = sectorRadius * glm::sin(j * sectorStep);

			vertexData.push_back(x);
			vertexData.push_back(z);
			vertexData.push_back(y);

			vertexData.push_back(x / radius);
			vertexData.push_back(z / radius);
			vertexData.push_back(y / radius);
		}
	}
}

void Sphere::CreateIndices()
{
	std::vector<unsigned int>().swap(indices);

	unsigned int sid;
	unsigned int nsid;

	for (int i = 0; i < stacks; i++)
	{
		sid = i * (sectors + 1);
		nsid = (i + 1) * (sectors + 1);

		for (int j = 0; j < sectors; j++)
		{
			if (i != 0)
			{
				indices.push_back(sid);
				indices.push_back(nsid);
				indices.push_back(sid + 1);
			}

			if (i != stacks - 1)
			{
				indices.push_back(sid + 1);
				indices.push_back(nsid);
				indices.push_back(nsid + 1);
			}

			sid++;
			nsid++;
		}
	}
}