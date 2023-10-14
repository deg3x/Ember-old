#include "MaterialUnlit.h"

#include "../Shader.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../../utils/PathBuilder.h"

MaterialUnlit::MaterialUnlit()
{
    shader = std::make_shared<Shader>(
        PathBuilder::GetPath("./Engine/shaders/vertexUnlit.glsl").c_str(),
        PathBuilder::GetPath("./Engine/shaders/fragmentUnlit.glsl").c_str()
        );
}

void MaterialUnlit::Use() const
{
}

void MaterialUnlit::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    GetShader()->SetVector4("color", diffuseColor);
}
