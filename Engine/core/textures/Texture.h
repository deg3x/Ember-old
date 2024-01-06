#pragma once

#include "Definitions.h"

#include <string>

enum class TextureFormat
{
    // Values taken by the equivalent GLAD "enums" for API compliance
    R = 0x1903,
    RGB = 0x1907,
    RGBA = 0x1908
};

class ENGINE_API Texture
{
protected:
    unsigned int textureID;
    TextureFormat format;
    std::string path;
    
public:
    Texture() {}
    virtual ~Texture() {}
    
    virtual void Use() const = 0;

    inline TextureFormat GetFormat() const
    {
        return format;
    }
    
    inline std::string GetPath() const
    {
        return path;
    }
    
protected:
    virtual void InitializeTexture(const char* texturePath) = 0;
};
