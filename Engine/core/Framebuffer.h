#pragma once

#include "../Definitions.h"

class ENGINE_API Framebuffer
{
private:
    unsigned int fbo;
    
public:
    Framebuffer();
    virtual ~Framebuffer();
    
    void Use();
};
