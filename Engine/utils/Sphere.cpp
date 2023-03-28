#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Sphere.h"
#include "Types.h"

#include <vector>

Sphere::Sphere()
{
	sectors = 10;
	stacks = 10;
	radius = 0.5f;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Sphere::Sphere(int initSectors, int initStacks, float initRadius)
{
	sectors = initSectors < 4 ? 4 : initSectors;
	stacks = initStacks < 3 ? 3 : initStacks;
	radius = initRadius;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Sphere::~Sphere()
{

}

void Sphere::GenerateVertexData()
{
	std::vector<VertexData>().swap(vertexData);

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

			VertexData currentVertex;

			currentVertex.position = glm::vec3(x, y, z);
			currentVertex.normal = glm::normalize(currentVertex.position);
			currentVertex.uv = glm::vec2((float)j / (float)sectors, (float)i / (float)stacks);

			vertexData.push_back(currentVertex);
		}
	}
}

void Sphere::GenerateIndices()
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
