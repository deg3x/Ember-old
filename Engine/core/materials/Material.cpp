#include "engine_pch.h"
#include "Material.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"

void Material::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();

    const glm::mat4x4 modelMatrix  = objectTransform.GetModelMatrix();
    const glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
    shader->SetMatrix4x4("normalMatrix", normalMatrix);
    shader->SetVector3("cameraPosition", camera.GetOwner()->transform->position);
}
