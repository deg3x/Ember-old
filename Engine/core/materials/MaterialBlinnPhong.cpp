#include "engine_pch.h"
#include "MaterialBlinnPhong.h"

#include "core/Shader.h"
#include "core/textures/TextureDiffuse.h"
#include "utils/PathBuilder.h"

MaterialBlinnPhong::MaterialBlinnPhong()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str()
        );

    diffuseTexture = std::make_shared<TextureDiffuse>();
}

MaterialBlinnPhong::MaterialBlinnPhong(const std::shared_ptr<TextureDiffuse>& texture)
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str()
        );
    
    diffuseTexture = texture;
}

void MaterialBlinnPhong::Use() const
{
    diffuseTexture->Use();
}

void MaterialBlinnPhong::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    Material::SetupShaderVariables(objectTransform, camera);
    
    shader->SetVector4("material.diffuse", diffuseColor);
    shader->SetVector3("material.specular", specular);
    shader->SetFloat("material.shininess", shininessExponent);
    shader->SetInt("diffuseTexture", 0);
}
