#pragma once

#include "../Definitions.h"

struct WindowData;

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    
public:
    Framebuffer() = delete;
    Framebuffer(const WindowData& windowData);
    virtual ~Framebuffer();
    
    void Bind();
    
    static void Unbind();
};
