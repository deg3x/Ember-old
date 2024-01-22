#include "Mesh.h"

#include "glad/glad.h"

#include "core/components/Camera.h"
#include "core/components/lights/Light.h"
#include "core/materials/Material.h"
#include "core/objects/Object.h"
#include "logger/Logger.h"

Mesh::Mesh(const std::shared_ptr<Material>& initMaterial)
{
	material = initMaterial;
}

Mesh::Mesh(const std::vector<VertexData>& data, const std::vector<unsigned>& initIndices, const std::shared_ptr<Material>& initMaterial)
{
	vertexData = data;
	indices = initIndices;
	material = initMaterial;

	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) *vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) *indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Mesh::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
	if (material == nullptr)
	{
		Logger::Log(LogCategory::ERROR, "Mesh has no material", "Mesh::Draw");
		return;
	}
	
	material->Use();

	for (const std::shared_ptr<Light>& light : lights)
	{
		light->SetShaderProperties(*material->GetShader());
	}
	
	material->SetupShaderVariables(*GetOwner()->transform, *camera);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
