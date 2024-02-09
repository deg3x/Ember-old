#include "engine_pch.h"
#include "Framebuffer.h"

#include "Renderbuffer.h"
#include "glad/glad.h"

#include "RendererTypes.h"
#include "logger/Logger.h"

Framebuffer::Framebuffer(int initWidth, int initHeight)
{
    width  = initWidth;
    height = initHeight;
    
    glGenFramebuffers(1, &id);
    Bind();
    
    // Color texture attachment
    textureFB = std::make_unique<Texture>(TextureType::DIFFUSE);
    textureFB->Bind();

    textureFB->SetWidth(initWidth);
    textureFB->SetHeight(initHeight);
    
    // Render buffer object attachment for depth/stencil
    // Use a texture instead if we need to sample, since reading is suboptimal in Renderbuffers in favour of performance
    renderbuffer = std::make_unique<Renderbuffer>(initWidth, initHeight);

    // std::unique<Texture> texture = std::make_unique<Texture>(TextureType::DIFFUSE, TEX_0, DEPTH24_STENCIL8);
    // glFramebufferTexture2D(FRAMEBUFFER, DEPTH_STENCIL, TEXTURE_2D, texture->GetID(), 0);
    
    glFramebufferTexture2D(FRAMEBUFFER, COLOR_ATTACHMENT_0, TEXTURE_2D, textureFB->GetID(), 0);
    glFramebufferRenderbuffer(FRAMEBUFFER, DEPTH_STENCIL, RENDERBUFFER, renderbuffer->GetID());
    
    if (glCheckFramebufferStatus(FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::Log(LogCategory::ERROR, "Framebuffer is not complete", "Framebuffer::Framebuffer");
    }
    
    Unbind();
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

void Framebuffer::Resize(int newWidth, int newHeight)
{
    if (width == newWidth && height == newHeight)
    {
        return;
    }
    
    width  = newWidth;
    height = newHeight;

    textureFB->SetWidth(newWidth);
    textureFB->SetHeight(newHeight);
    textureFB->SetTextureData(nullptr);
    
    renderbuffer->Resize(newWidth, newHeight);

    Bind();
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFB->GetID(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GetID());
}
