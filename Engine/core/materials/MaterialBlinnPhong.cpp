#include "MaterialBlinnPhong.h"

#include "../Shader.h"
#include "../Texture.h"
#include "../objects/Object.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../../utils/PathBuilder.h"

MaterialBlinnPhong::MaterialBlinnPhong()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str()
        );

    diffuseTexture = std::make_shared<Texture>();
}

MaterialBlinnPhong::MaterialBlinnPhong(const std::shared_ptr<Texture>& texture) : MaterialBlinnPhong()
{
    diffuseTexture = texture;
}

void MaterialBlinnPhong::Use() const
{
    diffuseTexture->Use();
}

void MaterialBlinnPhong::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();

    const glm::mat4x4 modelMatrix  = objectTransform.GetModelMatrix();
    const glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
        
    shader->SetMatrix4x4("normalMatrix", normalMatrix);
    shader->SetVector3("cameraPosition", camera.GetParent()->transform->position);
    shader->SetVector4("material.diffuse", diffuseColor);
    shader->SetVector3("material.specular", specular);
    shader->SetFloat("material.shininess", shininessExponent);

    shader->SetInt("diffuseTexture", 0);
}
