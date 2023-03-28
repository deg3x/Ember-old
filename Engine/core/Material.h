#pragma once

class Shader;

class Material
{
protected:
    Shader* shader;

public:
    Material();
    Material(const char* vertShader, const char* fragShader);
    Material(Shader* shader);

    void SetShader(const char* vertShader, const char* fragShader);
    void SetShader(Shader* shader);

    inline Shader* GetShader() const
    {
        return shader;
    }
};
