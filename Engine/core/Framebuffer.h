#pragma once

#include "Definitions.h"
#include "RendererTypes.h"
#include "Texture.h"

class Renderbuffer;

class ENGINE_API Framebuffer
{
private:
    unsigned int id;
    
public:
    Framebuffer();
    virtual ~Framebuffer();
    
    void Bind() const;
    void Unbind() const;

    void SetTextureAttachment(const std::shared_ptr<Texture>& texture, RenderAttachment attachment, TextureTarget texTarget);
    void SetRenderBufferAttachment(const std::shared_ptr<Renderbuffer>& renderBuffer, RenderAttachment attachment);

    bool CheckIsComplete();
};
