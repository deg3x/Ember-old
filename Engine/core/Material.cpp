#include "Material.h"
#include "Shader.h"

Material::Material()
{
    // Dirty way to fight with Premake being unable to change the product scheme working directory for xcode
    
#if defined(_WIN32)
    shader = new Shader("./Engine/shaders/vertexPhong.shader", "./Engine/shaders/fragmentPhong.shader");
#elif __APPLE__
    shader = new Shader("../../Engine/shaders/vertexPhong.shader", "../../Engine/shaders/fragmentPhong.shader");
#endif
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
}

Material::~Material()
{
    if (shader == nullptr)
    {
        return;
    }

    delete shader;
    shader = nullptr;
}

void Material::SetShader(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);
}
