#pragma once

#include <memory>
#include <vector>

class Shader;
class Texture;

class Material
{
protected:
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

public:
    Material();
    Material(const char* vertShader, const char* fragShader);
    Material(const char* vertShader, const char* fragShader, const std::vector<std::shared_ptr<Texture>>& diffTextures);
    virtual ~Material() = default;

    void SetShader(const char* vertShader, const char* fragShader);
    void SetTextures(const std::vector<std::shared_ptr<Texture>>& diffTextures);
    void Use() const;

    inline std::shared_ptr<Shader> GetShader() const
    {
        return shader;
    }

    inline std::vector<std::shared_ptr<Texture>> GetTextures() const
    {
        return textures;
    }

protected:
    void SetupShaderVariables() const;
};