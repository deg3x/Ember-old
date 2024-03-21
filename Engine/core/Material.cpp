#include "engine_pch.h"
#include "Material.h"

#include "glad/glad.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"

Material::Material(const std::shared_ptr<Shader>& initShader)
{
    shader = initShader;
}

void Material::Use() const
{
    for (const MaterialTexture& texture : textures)
    {
        texture.texture->Bind();
    }
}

void Material::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();

    const glm::mat4x4 modelMatrix  = objectTransform.modelMatrix;
    const glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

    shader->SetMatrix4x4("model", modelMatrix);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
    shader->SetMatrix4x4("normalMatrix", normalMatrix);
    shader->SetVector3("cameraPosition", camera.GetOwner()->transform->position);

    for (const MaterialTexture& matTexture : textures)
    {
        shader->SetInt(matTexture.name, matTexture.texture->GetUnit() - TEX_0);
    }

    for (const MaterialProperty& property : properties)
    {
        switch (property.type)
        {
        case MaterialProperty::PropertyType::INTEGER:
            shader->SetInt(property.name, property.value.intVal);
            break;
        case MaterialProperty::PropertyType::BOOLEAN:
            shader->SetBool(property.name, property.value.boolVal);
            break;
        case MaterialProperty::PropertyType::FLOAT:
            shader->SetFloat(property.name, property.value.floatVal);
            break;
        case MaterialProperty::PropertyType::VECTOR3:
            shader->SetVector3(property.name, property.value.vector3Val);
            break;
        case MaterialProperty::PropertyType::VECTOR4:
            shader->SetVector4(property.name, property.value.vector4Val);
            break;
        case MaterialProperty::PropertyType::MATRIX4X4:
            shader->SetMatrix4x4(property.name, property.value.matrixVal);
            break;
        }
    }
}

void Material::SetTexture(const std::string& name, const std::string& path, TextureType type)
{
    textures.insert(MaterialTexture(name, std::make_shared<Texture>(path, type)));
}

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
{
    textures.insert(MaterialTexture(name, texture));
}

void Material::SetBool(const std::string& name, bool value) const
{
    shader->SetBool(name, value);
}

void Material::SetInt(const std::string& name, int value) const
{
    shader->SetInt(name, value);
}

void Material::SetFloat(const std::string& name, float value) const
{
    shader->SetFloat(name, value);
}

void Material::SetVector3(const std::string& name, const glm::vec3& vector) const
{
    shader->SetVector3(name, vector);
}

void Material::SetVector4(const std::string& name, const glm::vec4& vector) const
{
    shader->SetVector4(name, vector);
}

void Material::SetMatrix4x4(const std::string& name, const glm::mat4x4& matrix) const
{
    shader->SetMatrix4x4(name, matrix);
}
