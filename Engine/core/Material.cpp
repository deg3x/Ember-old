#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "../utils/PathBuilder.h"

Material::Material()
{
    shader = std::make_shared<Shader>(PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(), PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str());
    diffuseTexture = std::make_shared<Texture>();

    SetupShaderVariables();
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = std::make_shared<Shader>(vertShader, fragShader);
    diffuseTexture = std::make_shared<Texture>();

    SetupShaderVariables();
}

void Material::SetShader(const char* vertShader, const char* fragShader)
{
    // Perhaps we should check if the shaders are the same before re-allocating?
    shader.reset();
    shader = std::make_shared<Shader>(vertShader, fragShader);
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
