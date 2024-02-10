#include "engine_pch.h"
#include "RenderBuffer.h"

#include "glad/glad.h"

#include "RendererTypes.h"
#include "TextureTypes.h"

RenderBuffer::RenderBuffer(int initWidth, int initHeight, TextureFormat rbFormat)
{
    width  = initWidth;
    height = initHeight;
    format = rbFormat;

    glGenRenderbuffers(1, &id);
    
    Bind();
    
    glRenderbufferStorage(RENDERBUFFER, format, initWidth, initHeight);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::Bind() const
{
    glBindRenderbuffer(RENDERBUFFER, id);
}

void RenderBuffer::Unbind() const
{
    glBindRenderbuffer(RENDERBUFFER, 0);
}

void RenderBuffer::Resize(int newWidth, int newHeight)
{
    if (width == newWidth && height == newHeight)
    {
        return;
    }

    width  = newWidth;
    height = newHeight;

    Bind();
    glRenderbufferStorage(RENDERBUFFER, format, newWidth, newHeight);
}
