#pragma once

#include "../Definitions.h"

struct WindowData;

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    unsigned int textureColor;
    unsigned int rbo;
    
public:
    Framebuffer() = delete;
    Framebuffer(const WindowData& windowData);
    virtual ~Framebuffer();
    
    void Bind() const;
    
    static void Unbind();
};
