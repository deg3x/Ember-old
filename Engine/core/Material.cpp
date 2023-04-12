#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "../utils/PathBuilder.h"

Material::Material()
{
    shader = new Shader(PathBuilder::GetPath("./Engine/shaders/vertexPhong.shader").c_str(), PathBuilder::GetPath("./Engine/shaders/fragmentPhong.shader").c_str());
    
    diffuseTexture = new Texture();
    SetupShaderVariables();
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
    diffuseTexture = new Texture();

    SetupShaderVariables();
}

Material::~Material()
{
    if (shader == nullptr)
    {
        return;
    }

    delete shader;
    shader = nullptr;

    if (diffuseTexture == nullptr)
    {
        return;
    }

    delete diffuseTexture;
    diffuseTexture = nullptr;
}

void Material::SetShader(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
}

void Material::Use() const
{
    diffuseTexture->Use();
    shader->Use();
}

void Material::SetupShaderVariables() const
{
    shader->Use();
    shader->SetInt("diffuseTexture", 0);
}
