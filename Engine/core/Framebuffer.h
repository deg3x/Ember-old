#pragma once

#include "Definitions.h"
#include "Texture.h"

class Renderbuffer;

class ENGINE_API Framebuffer
{
private:
    unsigned int id;
    int width;
    int height;

    std::unique_ptr<Renderbuffer> renderbuffer;
    std::unique_ptr<Texture> textureFB;
    
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
        return width;
    }

    inline int GetHeight() const
    {
        return height;
    }
};
