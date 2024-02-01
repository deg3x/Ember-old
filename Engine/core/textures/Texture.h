#pragma once

#include "Definitions.h"

#include <string>

enum class TextureFormat : uint16_t
{
    // Values taken by the equivalent GLAD "enums" for API compliance
    R = 0x1903,
    RGB = 0x1907,
    RGBA = 0x1908
};

enum class TextureType : uint8_t
{
    DIFFUSE,
    CUBEMAP
};

enum TextureUnit : uint8_t
{
    TEX_0  = 0x00,
    TEX_1  = 0x01,
    TEX_2  = 0x02,
    TEX_3  = 0x03,
    TEX_4  = 0x04,
    TEX_5  = 0x05,
    TEX_6  = 0x06,
    TEX_7  = 0x07,
    TEX_8  = 0x08,
    TEX_9  = 0x09,
    TEX_10 = 0x0A,
    TEX_11 = 0x0B,
    TEX_12 = 0x0C,
    TEX_13 = 0x0D,
    TEX_14 = 0x0E,
    TEX_15 = 0x0F,
};

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
};
