#include "Texture.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include "../utils/PathBuilder.h"

#include <iostream>

Texture::Texture()
{
    InitializeTexture(PathBuilder::GetPath("./Data/images/container.jpg").c_str());
    
    type = TextureType::diffuse;
    path = "./Data/images/container.jpg";
}

Texture::Texture(TextureType inType, const char* texturePath)
{
    type = inType;
    path = texturePath;
    InitializeTexture(texturePath);
}

void Texture::Use() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::InitializeTexture(const char* texturePath)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(texturePath, &width, &height, &nChannels, 0);

    if (!data)
    {
        std::cerr << "[Texture] Error while trying to parse texture..." << std::endl;
        return;
    }

    switch (nChannels)
    {
    case 1:
        format = TextureFormat::R;
        break;
    case 3:
        format = TextureFormat::RGB;
        break;
    case 4:
        format = TextureFormat::RGBA;
        break;
    default:
        std::cerr << "[Texture] Unknown number of channels while trying to parse texture..." << std::endl;
        return;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, static_cast<GLint>(format), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
}
