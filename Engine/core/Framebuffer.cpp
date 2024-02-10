#include "engine_pch.h"
#include "Framebuffer.h"

#include "Renderbuffer.h"
#include "glad/glad.h"

#include "RendererTypes.h"
#include "logger/Logger.h"

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &id);
    
    // Render buffer object attachment for depth/stencil
    // Use a texture instead if we need to sample, since reading is suboptimal in Renderbuffers in favour of performance

    // std::unique<Texture> texture = std::make_unique<Texture>(TextureType::DIFFUSE, TEX_0, DEPTH24_STENCIL8);
    // glFramebufferTexture2D(FRAMEBUFFER, DEPTH_STENCIL, TEXTURE_2D, texture->GetID(), 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &id);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::SetTextureAttachment(const std::shared_ptr<Texture>& texture, RenderAttachment attachment, TextureTarget texTarget)
{
    Bind();
    
    glFramebufferTexture2D(FRAMEBUFFER, attachment, texTarget, texture->GetID(), 0);
}

void Framebuffer::SetRenderBufferAttachment(const std::shared_ptr<Renderbuffer>& renderBuffer, RenderAttachment attachment)
{
    Bind();

    glFramebufferRenderbuffer(FRAMEBUFFER, DEPTH_STENCIL, RENDERBUFFER, renderBuffer->GetID());
}

bool Framebuffer::CheckIsComplete()
{
    Bind();
    
    return (glCheckFramebufferStatus(FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
}
