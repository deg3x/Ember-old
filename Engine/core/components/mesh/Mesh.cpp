#include "Mesh.h"

#include "glad/glad.h"
#include "../Transform.h"
#include "../../Shader.h"
#include "../../Material.h"
#include "../../objects/Object.h"

#include <iostream>

Mesh::Mesh(const std::shared_ptr<Material>& initMaterial)
{
	material = initMaterial;
}

Mesh::Mesh(std::vector<VertexData> data, std::vector<unsigned> initIndices, const std::shared_ptr<Material>& initMaterial)
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

void Mesh::Draw() const
{
	if (material == nullptr)
	{
		std::cerr << "[!] Mesh has no material!" << std::endl;
		return;
	}

	material->Use();

	const glm::mat4x4 model = GetParent()->transform->GetModelMatrix();
	const glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(model));
	material->GetShader()->SetMatrix4x4("model", model);
	material->GetShader()->SetMatrix4x4("normalMatrix", normalMatrix);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
