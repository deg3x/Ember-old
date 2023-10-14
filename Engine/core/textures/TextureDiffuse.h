#pragma once

#include "Texture.h"

class ENGINE_API TextureDiffuse : public Texture
{
public:
    TextureDiffuse();
    TextureDiffuse(const char* texturePath);

    void Use() const override;

protected:
    void InitializeTexture(const char* texturePath) override;
};
