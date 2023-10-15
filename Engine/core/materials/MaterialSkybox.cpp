#include "MaterialSkybox.h"

#include "../Shader.h"
#include "../textures/TextureCubemap.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../../utils/PathBuilder.h"

MaterialSkybox::MaterialSkybox()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexSkybox.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentSkybox.glsl").c_str()
        );

    cubemap = std::make_shared<TextureCubemap>();
}

MaterialSkybox::MaterialSkybox(const std::shared_ptr<TextureCubemap> texture)
{
    cubemap = texture;
}

void MaterialSkybox::Use() const
{
    cubemap->Use();
}

void MaterialSkybox::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();
    
    const glm::mat4x4 modelMatrix = objectTransform.GetModelMatrix();
    const glm::mat4x4 viewMatrix  = glm::mat4(glm::mat3(camera.GetViewMatrix())); // Camera position independence

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", viewMatrix);
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());

    shader->SetInt("skybox", 0);
}
