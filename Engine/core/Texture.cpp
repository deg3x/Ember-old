#include "Texture.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include "../utils/PathBuilder.h"

#include <iostream>

Texture::Texture()
{
    InitializeTexture(PathBuilder::GetPath("./Data/images/white.png").c_str());
    
    type = TextureType::diffuse;
    path = "./Data/images/white.png";
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
    if (type == TextureType::cubemap)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

void Texture::InitializeTexture(const char* texturePath)
{
    if (type == TextureType::cubemap)
    {
        InitializeCubemap(texturePath);
    }
    else
    {
        InitializeTexture2D(texturePath);
    }
}

void Texture::InitializeCubemap(const char* texturePath)
{
    int width, height, nChannels;

    const std::string faces[6] = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < 6; i++)
    {
        std::string fullPath = texturePath;
        fullPath += faces[i];
            
        unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << fullPath << std::endl;
        }
        
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::InitializeTexture2D(const char* texturePath)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nChannels, 0);
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
