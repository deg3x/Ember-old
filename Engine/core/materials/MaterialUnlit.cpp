#include "MaterialUnlit.h"

#include "core/Shader.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "core/textures/TextureDiffuse.h"
#include "utils/PathBuilder.h"

MaterialUnlit::MaterialUnlit()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexUnlit.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentUnlit.glsl").c_str()
        );

    diffuseTexture = std::make_shared<TextureDiffuse>();
}

MaterialUnlit::MaterialUnlit(const std::shared_ptr<TextureDiffuse>& texture)
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexUnlit.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentUnlit.glsl").c_str()
        );

    diffuseTexture = texture;
}

void MaterialUnlit::Use() const
{
    diffuseTexture->Use();
}

void MaterialUnlit::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();

    shader->Use();

    const glm::mat4x4 modelMatrix  = objectTransform.GetModelMatrix();

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
    
    GetShader()->SetVector4("color", diffuseColor);

    shader->SetInt("diffuseTexture", 0);
}
