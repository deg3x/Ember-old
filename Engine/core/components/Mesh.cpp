#include "engine_pch.h"
#include "Mesh.h"

#include "glad/glad.h"

#include "core/Material.h"
#include "core/Renderer.h"

Mesh::Mesh(const std::vector<VertexData>& data, const std::vector<unsigned int>& initIndices, const std::shared_ptr<Material>& initMaterial)
{
    vertexData = data;
    indices = initIndices;
    material = initMaterial;

    SetupMesh();
}

Mesh::~Mesh()
{
    CleanupMesh();
}

void Mesh::SetMeshData(const std::vector<VertexData>& newData, const std::vector<unsigned int>& newIndices, const std::shared_ptr<Material>& newMaterial)
{
    CleanupMesh();

    std::vector<VertexData>().swap(vertexData);
    std::vector<unsigned int>().swap(indices);

    vertexData = newData;
    indices    = newIndices;

    if (newMaterial != nullptr)
    {
        material = newMaterial;
    }
    
    SetupMesh();
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, bitangent));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}

void Mesh::CleanupMesh()
{
    if (glIsVertexArray(VAO))
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (glIsBuffer(VBO))
    {
        glDeleteBuffers(1, &VBO);
    }
    if (glIsBuffer(EBO))
    {
        glDeleteBuffers(1, &EBO);
    }
}

void Mesh::SetupDepthTestMode() const
{
    Renderer::SetDepthTestMask(writeToDepthBuffer);
    
    switch(depthTest)
    {
    case DepthTestMode::LESS:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_LESS);
        break;
    case DepthTestMode::LEQUAL:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_LEQUAL);
        break;
    case DepthTestMode::EQUAL:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_EQUAL);
        break;
    case DepthTestMode::GEQUAL:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_GEQUAL);
        break;
    case DepthTestMode::GREATER:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_GREATER);
        break;
    case DepthTestMode::NOT_EQUAL:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_NOTEQUAL);
        break;
    case DepthTestMode::ALWAYS:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_ALWAYS);
        break;
    case DepthTestMode::NEVER:
        Renderer::SetDepthTestEnabled(true);
        Renderer::SetDepthTestFunc(GL_NEVER);
        break;
    case DepthTestMode::NONE:
        Renderer::SetDepthTestEnabled(false);
        break;
    }
}

void Mesh::SetupPolygonMode() const
{
    switch(polygonMode)
    {
    case PolygonMode::FILL:
        Renderer::SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case PolygonMode::LINE:
        Renderer::SetPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }
}

void Mesh::SetupCullingMode() const
{
    switch(cullingMode)
    {
    case CullingMode::BACK:
        Renderer::SetFaceCullingEnabled(true);
        Renderer::SetFaceCullingMode(GL_BACK);
        break;
    case CullingMode::FRONT:
        Renderer::SetFaceCullingEnabled(true);
        Renderer::SetFaceCullingMode(GL_FRONT);
        break;
    case CullingMode::NONE:
        Renderer::SetFaceCullingEnabled(false);
        break;
    }
}

void Mesh::ResetRendererState() const
{
    Renderer::SetDepthTestMask(true);
    Renderer::SetDepthTestEnabled(true);
    Renderer::SetDepthTestFunc(GL_LESS);
    Renderer::SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Renderer::SetFaceCullingEnabled(false);
}
