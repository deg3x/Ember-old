#pragma once

#include "Definitions.h"
#include "TextureTypes.h"

class ENGINE_API Texture
{
private:
    std::string path;
    unsigned int textureID;
    TextureFormat formatImage;
    TextureFormat formatSaved;
    TextureType type;
    TextureUnit unit;
    
public:
    Texture() = delete;
    Texture(TextureType texType, TextureUnit texUnit = TEX_0, TextureFormat savedFormat = RGB, TextureFormat imageFormat = RGB);
    Texture(const std::string& texPath, TextureType texType, TextureUnit texUnit = TEX_0, TextureFormat savedFormat = RGB, TextureFormat imageFormat = RGB);
    
    void Bind() const;

    static TextureFormat ChannelsToFormat(int channels);

    inline TextureFormat GetFormat() const
    {
        return formatImage;
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
    void InitializeTextureCubeMap(const char* texturePath);
    void InitializeTextureHDR(const char* texturePath);

    static uint8_t* LoadImage(const char* path, int& width, int& height, int& channels, bool flip = true);
    static float* LoadImageFloat(const char* path, int& width, int& height, int& channels, bool flip = true);
};
