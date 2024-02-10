#pragma once

#include "Definitions.h"
#include "TextureTypes.h"

class ENGINE_API RenderBuffer
{
private:
    unsigned int id;
    int width;
    int height;

    TextureFormat format;
    
public:
    RenderBuffer() = delete;
    RenderBuffer(int initWidth, int initHeight, TextureFormat rbFormat = DEPTH24_STENCIL8);
    virtual ~RenderBuffer();

    void Bind() const;
    void Unbind() const;
    void Resize(int newWidth, int newHeight);

    inline int GetWidth() const
    {
        return width;
    }

    inline int GetHeight() const
    {
        return height;
    }

    inline unsigned int GetID() const
    {
        return id;
    }
};
