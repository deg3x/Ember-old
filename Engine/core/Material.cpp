#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material()
{
    // Dirty way to fight with Premake being unable to change the product scheme working directory for xcode
#if defined(_WIN32)
    shader = new Shader("./Engine/shaders/vertexPhong.shader", "./Engine/shaders/fragmentPhong.shader");
#elif __APPLE__
    shader = new Shader("../../Engine/shaders/vertexPhong.shader", "../../Engine/shaders/fragmentPhong.shader");
#endif
    
    diffuseTexture = new Texture();
    SetupShaderVariables();
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
    diffuseTexture = new Texture();

    SetupShaderVariables();
}

Material::~Material()
{
    if (shader == nullptr)
    {
        return;
    }

    delete shader;
    shader = nullptr;

    if (diffuseTexture == nullptr)
    {
        return;
    }

    delete diffuseTexture;
    diffuseTexture = nullptr;
}

void Material::SetShader(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
}

void Material::Use() const
{
    diffuseTexture->Use();
    shader->Use();
}

void Material::SetupShaderVariables() const
{
    shader->Use();
    shader->SetInt("diffuseTexture", 0);
}
