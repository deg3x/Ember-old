#pragma once

class Shader;
class Texture;

//
// Currently we do not allow creating multiple materials with the same shader instance
// to prevent memory deref apocalypse. Improve in the future!
// 
class Material
{
protected:
    Shader* shader;
    Texture* diffuseTexture;

public:
    Material();
    Material(const char* vertShader, const char* fragShader);
    virtual ~Material();

    void SetShader(const char* vertShader, const char* fragShader);
    void Use() const;

    inline Shader* GetShader() const
    {
        return shader;
    }

protected:
    void SetupShaderVariables() const;
};