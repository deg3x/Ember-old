#pragma once

#include "Definitions.h"
#include "RendererTypes.h"

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    unsigned int rbo;
    unsigned int textureID;
    
    RenderbufferType typeRB;
    FramebufferAttachment attachmentFB;

    int currentWidth;
    int currentHeight;
    
public:
    Framebuffer() = delete;
    Framebuffer(int initWidth, int initHeight, FramebufferAttachment fbAttachment = FramebufferAttachment::DEPTH_STENCIL, RenderbufferType rbType = RenderbufferType::DEPTH24_STENCIL8);
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
