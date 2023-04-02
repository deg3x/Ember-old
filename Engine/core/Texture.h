#pragma once

enum class TextureType
{
    diffuse,
    normal
};

class Texture
{
private:
    TextureType type;
    unsigned int textureID;
    
public:
    Texture();
    Texture(TextureType inType, const char* texturePath);
    virtual ~Texture() = default;
    
    void Use() const;

    inline TextureType GetTextureType() const
    {
        return type;
    }
    
private:
    void InitializeTexture(const char* texturePath);
};
