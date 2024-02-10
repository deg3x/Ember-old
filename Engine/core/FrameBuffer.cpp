#include "engine_pch.h"
#include "FrameBuffer.h"

#include "RenderBuffer.h"
#include "glad/glad.h"

#include "RendererTypes.h"
#include "logger/Logger.h"

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &id);
    
    // Render buffer object attachment for depth/stencil
    // Use a texture instead if we need to sample, since reading is suboptimal in Renderbuffers in favour of performance

    // std::unique<Texture> texture = std::make_unique<Texture>(TextureType::DIFFUSE, TEX_0, DEPTH24_STENCIL8);
    // glFramebufferTexture2D(FRAMEBUFFER, DEPTH_STENCIL, TEXTURE_2D, texture->GetID(), 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetTextureAttachment(const std::shared_ptr<Texture>& texture, RenderAttachment attachment, TextureTarget texTarget)
{
    Bind();
    
    glFramebufferTexture2D(FRAMEBUFFER, attachment, texTarget, texture->GetID(), 0);
}

void FrameBuffer::SetRenderBufferAttachment(const std::shared_ptr<RenderBuffer>& renderBuffer, RenderAttachment attachment)
{
    Bind();

    glFramebufferRenderbuffer(FRAMEBUFFER, attachment, RENDERBUFFER, renderBuffer->GetID());
}

bool FrameBuffer::CheckIsComplete()
{
    Bind();
    
    return (glCheckFramebufferStatus(FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
}
