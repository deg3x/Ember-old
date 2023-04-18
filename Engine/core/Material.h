#pragma once

#include <memory>

class Shader;
class Texture;

class Material
{
protected:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> diffuseTexture;

public:
    Material();
    Material(const char* vertShader, const char* fragShader);
    virtual ~Material() = default;

    void SetShader(const char* vertShader, const char* fragShader);
    void Use() const;

    inline std::shared_ptr<Shader> GetShader() const
    {
        return shader;
    }

protected:
    void SetupShaderVariables() const;
};