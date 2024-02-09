#include "engine_pch.h"
#include "Renderbuffer.h"

#include "glad/glad.h"

#include "RendererTypes.h"
#include "TextureTypes.h"

Renderbuffer::Renderbuffer(int initWidth, int initHeight)
{
    width  = initWidth;
    height = initHeight;

    glGenRenderbuffers(1, &id);
    
    Bind();
    
    glRenderbufferStorage(RENDERBUFFER, DEPTH24_STENCIL8, initWidth, initHeight);
    
    Unbind();
}

Renderbuffer::~Renderbuffer()
{
    glDeleteRenderbuffers(1, &id);
}

void Renderbuffer::Bind() const
{
    glBindRenderbuffer(RENDERBUFFER, id);
}

void Renderbuffer::Unbind() const
{
    glBindRenderbuffer(RENDERBUFFER, 0);
}

void Renderbuffer::Resize(int newWidth, int newHeight)
{
    if (width == newWidth && height == newHeight)
    {
        return;
    }

    width  = newWidth;
    height = newHeight;

    Bind();
    glRenderbufferStorage(RENDERBUFFER, DEPTH24_STENCIL8, newWidth, newHeight);
}
