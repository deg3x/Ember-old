#pragma once

#include "Definitions.h"
#include "RendererTypes.h"
#include "Texture.h"

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    unsigned int rbo;

    std::unique_ptr<Texture> textureFB;

    int currentWidth;
    int currentHeight;
    
public:
    Framebuffer() = delete;
    Framebuffer(int initWidth, int initHeight);
    virtual ~Framebuffer();
    
    void Bind() const;
    void Unbind() const;

    void Resize(int newWidth, int newHeight);

    inline unsigned int GetTextureID() const
    {
        return textureFB->GetID();
    }

    inline int GetWidth() const
    {
        return currentWidth;
    }

    inline int GetHeight() const
    {
        return currentHeight;
    }
};
