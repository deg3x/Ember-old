#include "Framebuffer.h"

#include <stdio.h>

#include "glad/glad.h"

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &fbo);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
