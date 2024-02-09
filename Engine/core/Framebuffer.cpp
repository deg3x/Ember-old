#include "engine_pch.h"
#include "Framebuffer.h"

#include "glad/glad.h"

#include "logger/Logger.h"

Framebuffer::Framebuffer(int initWidth, int initHeight)
{
    currentWidth  = initWidth;
    currentHeight = initHeight;
    
    glGenFramebuffers(1, &fbo);
    Bind();
    
    // Color texture attachment
    textureFB = std::make_unique<Texture>(TextureType::DIFFUSE);
    textureFB->Bind();

    textureFB->SetWidth(initWidth);
    textureFB->SetHeight(initHeight);
    
    // Render buffer object attachment for depth/stencil
    // Use a texture instead if we need to sample, since reading is suboptimal in Renderbuffers in favour of performance
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(RENDERBUFFER, rbo);
    glRenderbufferStorage(RENDERBUFFER, DEPTH24_STENCIL8, initWidth, initHeight);
    glBindRenderbuffer(RENDERBUFFER, 0);

    // std::unique<Texture> texture = std::make_unique<Texture>(TextureType::DIFFUSE, TEX_0, DEPTH24_STENCIL8);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texDepthStencil, 0);
    
    glFramebufferTexture2D(FRAMEBUFFER, COLOR_ATTACHMENT_0, TEXTURE_2D, textureFB->GetID(), 0);
    glFramebufferRenderbuffer(FRAMEBUFFER, DEPTH_STENCIL, RENDERBUFFER, rbo);
    
    if (glCheckFramebufferStatus(FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::Log(LogCategory::ERROR, "Framebuffer is not complete", "Framebuffer::Framebuffer");
    }
    
    Unbind();
}

Framebuffer::~Framebuffer()
{
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int newWidth, int newHeight)
{
    if (currentWidth == newWidth && currentHeight == newHeight)
    {
        return;
    }
    
    currentWidth  = newWidth;
    currentHeight = newHeight;

    textureFB->SetWidth(newWidth);
    textureFB->SetHeight(newHeight);
    textureFB->SetTextureData(nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFB->GetID(), 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}
