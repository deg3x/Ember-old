#include "Sphere.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../../../core/materials/Material.h"

#include <vector>

Sphere::Sphere(const std::shared_ptr<Material>& initMaterial) : Mesh(initMaterial)
{
	sectors = 10;
	stacks = 10;
	radius = 0.5f;
	
	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

Sphere::Sphere(int initSectors, int initStacks, float initRadius, const std::shared_ptr<Material>& initMaterial) : Mesh(initMaterial)
{
	sectors = initSectors < 4 ? 4 : initSectors;
	stacks = initStacks < 3 ? 3 : initStacks;
	radius = initRadius;

	GenerateVertexData();
	GenerateIndices();
	SetupMesh();
}

void Sphere::GenerateVertexData()
{
	std::vector<VertexData>().swap(vertexData);

	glm::vec3 vertPos;

	const float stackStep = glm::pi<float>() / (float)stacks;
	const float sectorStep = 2.0f * glm::pi<float>() / (float)sectors;

	for (int i = 0; i <= stacks; i++)
	{
		const float stackAngle = (glm::pi<float>() * 0.5f) - (i * stackStep);
		const float sectorRadius = radius * glm::cos(stackAngle);
		vertPos.z = radius * glm::sin(stackAngle);

		for (int j = 0; j <= sectors; j++)
		{
			vertPos.x = sectorRadius * glm::cos(j * sectorStep);
			vertPos.y = sectorRadius * glm::sin(j * sectorStep);

			VertexData currentVertex;

			currentVertex.position = glm::vec3(vertPos.x, vertPos.y, vertPos.z);
			currentVertex.normal = glm::normalize(currentVertex.position);
			currentVertex.uv = glm::vec2((float)j / (float)sectors, (float)i / (float)stacks);

			vertexData.push_back(currentVertex);
		}
	}
}

void Sphere::GenerateIndices()
{
	std::vector<unsigned int>().swap(indices);

	for (int i = 0; i < stacks; i++)
	{
		unsigned int sid = i * (sectors + 1);
		unsigned int nsid = (i + 1) * (sectors + 1);

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
