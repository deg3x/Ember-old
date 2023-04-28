#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "../utils/PathBuilder.h"

Material::Material()
{
    shader = std::make_shared<Shader>(PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl").c_str(), PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl").c_str());
    textures.push_back(std::make_shared<Texture>());

    SetupShaderVariables();
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = std::make_shared<Shader>(vertShader, fragShader);
    textures.push_back(std::make_shared<Texture>());

    SetupShaderVariables();
}

Material::Material(const char* vertShader, const char* fragShader, const std::vector<std::shared_ptr<Texture>>& diffTextures)
{
    shader = std::make_shared<Shader>(vertShader, fragShader);

    
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

void Material::Use() const
{
    // Only use the first available diffuse texture for now... Improve in the future
    for (std::shared_ptr<Texture> tex : textures)
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

void Material::SetupShaderVariables() const
{
    shader->Use();
    shader->SetInt("diffuseTexture", 0);
}
