#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "objects/Object.h"
#include "components/Camera.h"
#include "components/Transform.h"
#include "../utils/PathBuilder.h"

namespace
{
    MaterialProperties* MakeMaterialPropertiesFromType(MaterialType type)
    {
        switch (type)
        {
        case MaterialType::Unlit:
            return new MaterialPropertiesUnlit();
        case MaterialType::Phong:
            return new MaterialPropertiesPhong();
        default:
            return nullptr;
        }
    }
}

Material::Material()
{
    type       = MaterialType::Phong;
    properties = MakeMaterialPropertiesFromType(type);
    shader     = std::make_shared<Shader>(PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(), PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str());

    textures.push_back(std::make_shared<Texture>());

    shader->SetInt("diffuseTexture", 0);
}

Material::Material(const char* vertShader, const char* fragShader, MaterialType matType)
{
    type       = matType;
    properties = MakeMaterialPropertiesFromType(type);
    shader     = std::make_shared<Shader>(vertShader, fragShader);

    textures.push_back(std::make_shared<Texture>());

    shader->SetInt("diffuseTexture", 0);
}

Material::Material(const char* vertShader, const char* fragShader, MaterialType matType, const std::vector<std::shared_ptr<Texture>>& diffTextures)
{
    type       = matType;
    properties = MakeMaterialPropertiesFromType(type);
    shader     = std::make_shared<Shader>(vertShader, fragShader);

    for (std::shared_ptr<Texture> tex : diffTextures)
    {
        textures.push_back(tex);
    }

    shader->SetInt("diffuseTexture", 0);
}

Material::~Material()
{
    if (properties == nullptr)
    {
        return;
    }

    delete properties;
    properties = nullptr;
}

void Material::SetShader(const char* vertShader, const char* fragShader)
{
    // Perhaps we should check if the shaders are the same before re-allocating?
    shader.reset();
    shader = std::make_shared<Shader>(vertShader, fragShader);
}

void Material::SetTextures(const std::vector<std::shared_ptr<Texture>>& diffTextures)
{
    if (textures.empty())
    {
        return;
    }

    std::vector<std::shared_ptr<Texture>>().swap(textures);
    
    for (std::shared_ptr<Texture> tex : diffTextures)
    {
        textures.push_back(std::move(tex));
    }
}

void Material::SetProperties(const MaterialProperties* matProperties)
{
    switch (type)
    {
    case MaterialType::Unlit:
        {
            const MaterialPropertiesUnlit* propsUnlit = dynamic_cast<const MaterialPropertiesUnlit*>(matProperties);
            MaterialPropertiesUnlit* thisPropsUnlit = dynamic_cast<MaterialPropertiesUnlit*>(properties);

            thisPropsUnlit->diffuse = propsUnlit->diffuse;

            break;
        }
    case MaterialType::Phong:
        {
            const MaterialPropertiesPhong* propsPhong = dynamic_cast<const MaterialPropertiesPhong*>(matProperties);
            MaterialPropertiesPhong* thisPropsPhong = dynamic_cast<MaterialPropertiesPhong*>(properties);

            thisPropsPhong->diffuse           = propsPhong->diffuse;
            thisPropsPhong->specular          = propsPhong->specular;
            thisPropsPhong->shininessExponent = propsPhong->shininessExponent;

            break;
        }
    default:
        return;
    }
}

void Material::Use() const
{
    // Only use the first available diffuse texture for now... Improve in the future
    for (const std::shared_ptr<Texture>& tex : textures)
    {
        if (tex->GetTextureType() != TextureType::diffuse)
        {
            continue;
        }

        tex->Use();
        break;
    }

    shader->Use();
}

void Material::SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const
{
    shader->Use();
    
    const glm::mat4x4 model = objectTransform.GetModelMatrix();

    shader->SetMatrix4x4("model", model);
    shader->SetMatrix4x4("view", camera.GetViewMatrix());
    shader->SetMatrix4x4("projection", camera.GetProjectionMatrix());

    switch(type)
    {
    case MaterialType::Unlit:
        {
            const MaterialPropertiesUnlit* propsUnlit = dynamic_cast<const MaterialPropertiesUnlit*>(properties);

            GetShader()->SetVector4("color", propsUnlit->diffuse);
	    
            break;
        }
    case MaterialType::Phong:
        {
            const MaterialPropertiesPhong* propsPhong = dynamic_cast<const MaterialPropertiesPhong*>(properties);
            const glm::mat4x4 normalMatrix            = glm::transpose(glm::inverse(model));
        
            shader->SetMatrix4x4("normalMatrix", normalMatrix);
            shader->SetVector3("cameraPosition", camera.GetParent()->transform->position);
            shader->SetVector4("material.diffuse", propsPhong->diffuse);
            shader->SetVector3("material.specular", propsPhong->specular);
            shader->SetFloat("material.shininess", propsPhong->shininessExponent);
	    
            break;
        }
    default:
        break;
    }
}
