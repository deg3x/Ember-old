#pragma once

#include "../Definitions.h"

#include <string>

enum class TextureType
{
    diffuse,
    normal,
    specular,
    height,
    cubemap
};

enum class TextureFormat
{
    // Values taken by the equivalent GLAD "enums" for API compliance
    R = 0x1903,
    RGB = 0x1907,
    RGBA = 0x1908
};

class ENGINE_API Texture
{
private:
    TextureType type;
    TextureFormat format;
    unsigned int textureID;
    std::string path;
    
public:
    Texture();
    Texture(TextureType inType, const char* texturePath);
    virtual ~Texture() = default;
    
    void Use() const;

    inline TextureType GetTextureType() const
    {
        return type;
    }

    inline std::string GetPath() const
    {
        return path;
    }
    
private:
    void InitializeTexture(const char* texturePath);

    void InitializeCubemap(const char* texturePath);
    void InitializeTexture2D(const char* texturePath);
};
