#pragma once

class Shader;

//
// Currently we do not allow creating multiple materials with the same shader instance
// to prevent memory deref apocalypse. Improve in the future!
// 
class Material
{
protected:
    Shader* shader;
    unsigned int diffuseTexID;

public:
    Material();
    Material(const char* vertShader, const char* fragShader);
    Material(const char* vertShader, const char* fragShader, const char* diffuseTexturePath);
    virtual ~Material();

    void SetShader(const char* vertShader, const char* fragShader);
    void Use();

    inline Shader* GetShader() const
    {
        return shader;
    }

protected:
    void InitializeTexture(const char* imagePath);
    
};
