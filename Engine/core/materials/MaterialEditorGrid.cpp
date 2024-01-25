#include "engine_pch.h"
#include "MaterialEditorGrid.h"

#include "core/Shader.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "utils/PathBuilder.h"

MaterialEditorGrid::MaterialEditorGrid()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexEditorGrid.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentEditorGrid.glsl").c_str()
        );
}

void MaterialEditorGrid::Use() const
{
    
}

void MaterialEditorGrid::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();

    const glm::mat4x4 modelMatrix = objectTransform.GetModelMatrix();

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
}
