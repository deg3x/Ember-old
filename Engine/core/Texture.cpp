#include "engine_pch.h"
#include "Texture.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "logger/Logger.h"
#include "utils/PathBuilder.h"

Texture::Texture(TextureType texType, TextureUnit texUnit, TextureFormat savedFormat, TextureFormat imageFormat, TextureDataType texDataType, int texWidth, int texHeight)
{
    path        = "NO_FILE";
    type        = texType;
    unit        = texUnit;
    formatSaved = savedFormat;
    formatImage = imageFormat;
    dataType    = texDataType;
    width       = texWidth;
    height      = texHeight;

    InitializeTexture(nullptr);
}

Texture::Texture(const std::string& texPath, TextureType texType, TextureUnit texUnit, TextureFormat savedFormat, TextureFormat imageFormat, TextureDataType texDataType, int texWidth, int texHeight)
{
    path        = texPath;
    type        = texType;
    unit        = texUnit;
    formatSaved = savedFormat;
    formatImage = imageFormat;
    dataType    = texDataType;
    width       = texWidth;
    height      = texHeight;

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

void Texture::SetData(const void* data, TextureTarget target) const
{
    Bind();

    if (target == AUTO)
    {
        switch(type)
        {
        case TextureType::DIFFUSE:
            glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, dataType, data);
            return;
        case TextureType::CUBE_MAP:
            for (int i = 0; i < 6; i++)
            {
                glTexImage2D(TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formatSaved, width, height, 0, formatImage, dataType, data);
            }
            return;
        case TextureType::HDR:
            glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, dataType, data);
            return;
        }
    }

    glTexImage2D(target, 0, formatSaved, width, height, 0, formatImage, dataType, data);
}

void Texture::SetParameter(TextureTarget target, TextureParameterName paramName, TextureParameter paramValue) const
{
    Bind();
    
    glTexParameteri(target, paramName, paramValue);
}

void Texture::GenerateMipmap(TextureTarget target) const 
{
    Bind();
    
    glGenerateMipmap(target);
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
    int channels;

    if (texPath != nullptr)
    {
        data = LoadImage(texPath, width, height, channels);

        formatImage = ChannelsToFormat(channels);
    }

    glGenTextures(1, &textureID);
    Bind();

    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_S, REPEAT);
    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_T, REPEAT);
    glTexParameteri(TEXTURE_2D, TEXTURE_MIN_FILTER, LINEAR);
    glTexParameteri(TEXTURE_2D, TEXTURE_MAG_FILTER, LINEAR);

    glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, dataType, data);
    GenerateMipmap(TEXTURE_2D);

    if (data)
    {
        stbi_image_free(data);
    }
}

void Texture::InitializeTextureCubeMap(const char* texturePath)
{
    uint8_t *data = nullptr;

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
    Bind();
    
    for (unsigned int i = 0; i < 6; i++)
    {
        if (texturePath != nullptr)
        {
            std::string fullPath = texturePath;
            fullPath += faces[i];
            
            data = LoadImage(fullPath.c_str(), width, height, nChannels, false);
        }

        glTexImage2D(TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formatSaved, width, height, 0, formatImage, dataType, data);

        if (data)
        {
            stbi_image_free(data);
        }
    }
    
    glTexParameteri(TEXTURE_CUBE_MAP, TEXTURE_MIN_FILTER, LINEAR);
    glTexParameteri(TEXTURE_CUBE_MAP, TEXTURE_MAG_FILTER, LINEAR);
    glTexParameteri(TEXTURE_CUBE_MAP, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_CUBE_MAP, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_CUBE_MAP, TEXTURE_WRAP_R, CLAMP_TO_EDGE);
}

void Texture::InitializeTextureHDR(const char* texturePath)
{
    int nChannels;
    
    float *data = nullptr;
    if (texturePath != nullptr)
    {
        data = LoadImageFloat(texturePath, width, height, nChannels);
    }
    
    glGenTextures(1, &textureID);
    Bind();

    glTexImage2D(TEXTURE_2D, 0, formatSaved, width, height, 0, formatImage, dataType, data);

    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_2D, TEXTURE_MIN_FILTER, LINEAR);
    glTexParameteri(TEXTURE_2D, TEXTURE_MAG_FILTER, LINEAR);

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
