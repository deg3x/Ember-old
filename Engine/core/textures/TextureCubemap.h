#pragma once

#include "Texture.h"

class ENGINE_API TextureCubemap : public Texture
{
public:
    TextureCubemap();
    TextureCubemap(const char* texturePath);

    void Use() const override;

protected:
    void InitializeTexture(const char* texturePath) override;
};
