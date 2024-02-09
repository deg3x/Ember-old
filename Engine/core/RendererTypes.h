#pragma once

enum RenderbufferType
{
    DEPTH24          = 0x81A6,
    DEPTH24_STENCIL8 = 0x88F0,
};

enum FramebufferAttachment
{
    DEPTH         = 0x8D00,
    DEPTH_STENCIL = 0x821A,
};