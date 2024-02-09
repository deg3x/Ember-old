#include "engine_pch.h"
#include "Texture.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "logger/Logger.h"
#include "utils/PathBuilder.h"

Texture::Texture(TextureType texType, TextureUnit texUnit, TextureFormat savedFormat, TextureFormat imageFormat)
{
    path        = "NO_FILE";
    type        = texType;
    unit        = texUnit;
    formatSaved = savedFormat;
    formatImage = imageFormat;

    InitializeTexture(nullptr);
}

Texture::Texture(const std::string& texPath, TextureType texType, TextureUnit texUnit, TextureFormat savedFormat, TextureFormat imageFormat)
{
    path        = texPath;
    type        = texType;
    unit        = texUnit;
    formatSaved = savedFormat;
    formatImage = imageFormat;

    InitializeTexture(PathBuilder::GetPath(texPath).c_str());
}

void Texture::Bind() const
{
    glActiveTexture(unit);
    
    switch(type)
    {
    case TextureType::DIFFUSE:
        glBindTexture(TEXTURE_2D, textureID);
        break;
    case TextureType::CUBE_MAP:
        glBindTexture(TEXTURE_CUBE_MAP, textureID);
        break;
    case TextureType::HDR:
        glBindTexture(TEXTURE_2D, textureID);
        break;
    }
}

TextureFormat Texture::ChannelsToFormat(int channels)
{
    switch (channels)
    {
    case 1:
        return TextureFormat::R;
    case 3:
        return TextureFormat::RGB;
    case 4:
        return TextureFormat::RGBA;
    default:
        Logger::Log(LogCategory::ERROR, "Unknown number of channels while trying to parse texture", "TextureDiffuse::InitializeTexture");
        return TextureFormat::NONE;
    }
}

void Texture::InitializeTexture(const char* texPath)
{
    switch(type)
    {
    case TextureType::DIFFUSE:
        InitializeTextureDiffuse(texPath);
        break;
    case TextureType::CUBE_MAP:
        InitializeTextureCubeMap(texPath);
        break;
    case TextureType::HDR:
        InitializeTextureHDR(texPath);
        break;
    }
}

void Texture::InitializeTextureDiffuse(const char* texPath)
{
    uint8_t *data = nullptr;

    int width;
    int height;
    int channels;

    if (texPath != nullptr)
    {
        data = LoadImage(texPath, width, height, channels);

        formatImage = ChannelsToFormat(channels);
    }

    glGenTextures(1, &textureID);
    glBindTexture(TEXTURE_2D, textureID);

    glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(TEXTURE_2D);

    if (data)
    {
        stbi_image_free(data);
    }
}

void Texture::InitializeTextureCubeMap(const char* texturePath)
{
    uint8_t *data = nullptr;

    int width;
    int height;
    int nChannels;

    const std::string faces[6] = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    glGenTextures(1, &textureID);
    glBindTexture(TEXTURE_CUBE_MAP, textureID);
    
    for (unsigned int i = 0; i < 6; i++)
    {
        std::string fullPath = texturePath;
        fullPath += faces[i];

        if (texturePath != nullptr)
        {
            data = LoadImage(fullPath.c_str(), width, height, nChannels, false);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formatSaved, width, height, 0, formatImage, GL_UNSIGNED_BYTE, data);

        if (data)
        {
            stbi_image_free(data);
        }
    }
    
    glTexParameteri(TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::InitializeTextureHDR(const char* texturePath)
{
    int width;
    int height;
    int nChannels;
    
    float *data = nullptr;
    if (texturePath != nullptr)
    {
        data = LoadImageFloat(texturePath, width, height, nChannels);
    }
    
    glGenTextures(1, &textureID);
    glBindTexture(TEXTURE_2D, textureID);

    glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, GL_FLOAT, data);

    glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        stbi_image_free(data);
    }
}

uint8_t* Texture::LoadImage(const char* path, int& width, int& height, int& channels, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    
    uint8_t* imageData = stbi_load(path, &width, &height, &channels, 0);
    if (!imageData)
    {
        std::string errorMsg = "Unable to parse texture ";
        errorMsg += path;
        
        Logger::Log(LogCategory::ERROR, errorMsg, "TextureDiffuse::InitializeTexture");
        
        return nullptr;
    }

    return imageData;
}

float* Texture::LoadImageFloat(const char* path, int& width, int& height, int& channels, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    
    float* imageData = stbi_loadf(path, &width, &height, &channels, 0);
    if (!imageData)
    {
        std::string errorMsg = "Unable to parse texture ";
        errorMsg += path;
        
        Logger::Log(LogCategory::ERROR, errorMsg, "TextureDiffuse::InitializeTexture");
        
        return nullptr;
    }

    return imageData;
}
