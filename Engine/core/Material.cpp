#include "Material.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include <iostream>

#include "stb_image.h"

Material::Material()
{
    // Dirty way to fight with Premake being unable to change the product scheme working directory for xcode
#if defined(_WIN32)
    shader = new Shader("./Engine/shaders/vertexPhong.shader", "./Engine/shaders/fragmentPhong.shader");
    InitializeTexture("./Data/images/container.jpg");
#elif __APPLE__
    shader = new Shader("../../Engine/shaders/vertexPhong.shader", "../../Engine/shaders/fragmentPhong.shader");
    InitializeTexture("../../Data/images/container.jpg");
#endif
}

Material::Material(const char* vertShader, const char* fragShader)
{
    shader = new Shader(vertShader, fragShader);

#if defined(_WIN32)
    InitializeTexture("./Data/images/container.jpg");
#elif __APPLE__
    InitializeTexture("../../Data/images/container.jpg");
#endif
}

Material::Material(const char* vertShader, const char* fragShader, const char* diffuseTexturePath) : Material(vertShader, fragShader)
{
    InitializeTexture(diffuseTexturePath);
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

void Material::Use()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexID);
    
    shader->Use();
}

void Material::InitializeTexture(const char* imagePath)
{
    glGenTextures(1, &diffuseTexID);
    glBindTexture(GL_TEXTURE_2D, diffuseTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(imagePath, &width, &height, &nChannels, 0);

    if (!data)
    {
        return;
    }
    std::cout << nChannels << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    shader->Use();
    shader->SetInt("diffuseTexture", 0);
    
    stbi_image_free(data);
}
