#pragma once

#include "Definitions.h"
#include "TextureTypes.h"

class ENGINE_API Texture
{
private:
    std::string path;
    
    unsigned int textureID;
    int width;
    int height;
    
    TextureType type;
    TextureUnit unit;
    TextureFormat formatSaved;
    TextureFormat formatImage;
    TextureDataType dataType;
    
public:
    Texture() = delete;
    Texture(TextureType texType, TextureUnit texUnit = TEX_0, TextureFormat savedFormat = RGB, TextureFormat imageFormat = RGB, TextureDataType texDataType = UNSIGNED_BYTE, int texWidth = 0, int texHeight = 0);
    Texture(const std::string& texPath, TextureType texType, TextureUnit texUnit = TEX_0, TextureFormat savedFormat = RGB, TextureFormat imageFormat = RGB, TextureDataType texDataType = UNSIGNED_BYTE, int texWidth = 0, int texHeight = 0);
    
    void Bind() const;
    void SetData(const void* data, TextureTarget target = TextureTarget::AUTO) const;
    void SetParameter(TextureTarget target, TextureParameterName paramName, TextureParameter paramValue) const;

    void GenerateMipmap(TextureTarget target) const;

    static TextureFormat ChannelsToFormat(int channels);

    inline void SetUnit(TextureUnit newUnit)
    {
        unit = newUnit;
    }

    inline void SetFormatSaved(TextureFormat newFormat)
    {
        formatSaved = newFormat;
    }
    
    inline void SetDataType(TextureDataType newDataType)
    {
        dataType = newDataType;
    }
    
    inline void SetWidth(int newWidth)
    {
        width = newWidth;
    }

    inline void SetHeight(int newHeight)
    {
        height = newHeight;
    }

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

    inline unsigned int GetID() const
    {
        return textureID;
    }
    
private:
    void InitializeTexture(const char* texPath);
    void InitializeTextureDiffuse(const char* texPath);
    void InitializeTextureCubeMap(const char* texturePath);
    void InitializeTextureHDR(const char* texturePath);

    static uint8_t* LoadImage(const char* path, int& width, int& height, int& channels, bool flip = true);
    static float* LoadImageFloat(const char* path, int& width, int& height, int& channels, bool flip = true);
};
