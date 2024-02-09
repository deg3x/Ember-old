#pragma once

#include "Definitions.h"
#include "TextureTypes.h"

class ENGINE_API Texture
{
private:
    std::string path;
    unsigned int textureID;
    TextureFormat format;
    TextureType type;
    TextureUnit unit;
    
public:
    Texture() = delete;
    Texture(const std::string& texPath, TextureType texType, TextureUnit texUnit = TextureUnit::TEX_0);
    
    void Bind() const;

    inline TextureFormat GetFormat() const
    {
        return format;
    }

    inline TextureType GetType() const
    {
        return type;
    }

    inline TextureUnit GetUnit() const
    {
        return unit;
    }
    
    inline std::string GetPath() const
    {
        return path;
    }
    
private:
    void InitializeTexture(const char* texPath);
    void InitializeTextureDiffuse(const char* texPath);
    void InitializeTextureCubemap(const char* texturePath);
    void InitializeTextureHDR(const char* texturePath);
};
