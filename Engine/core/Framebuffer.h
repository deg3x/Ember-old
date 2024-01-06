#pragma once

#include "Definitions.h"

struct WindowData;

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    unsigned int textureID;
    unsigned int rbo;

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
        return textureID;
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
