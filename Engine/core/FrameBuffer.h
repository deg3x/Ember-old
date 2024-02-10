#pragma once

#include "Definitions.h"
#include "RendererTypes.h"
#include "Texture.h"

class RenderBuffer;

class ENGINE_API FrameBuffer
{
private:
    unsigned int id;
    
public:
    FrameBuffer();
    virtual ~FrameBuffer();
    
    void Bind() const;
    void Unbind() const;

    void SetTextureAttachment(const std::shared_ptr<Texture>& texture, RenderAttachment attachment, TextureTarget texTarget);
    void SetRenderBufferAttachment(const std::shared_ptr<RenderBuffer>& renderBuffer, RenderAttachment attachment);

    bool CheckIsComplete();
};
