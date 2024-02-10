#include "engine_pch.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "Texture.h"

#include "window/Window.h"
#include "utils/Types.h"
#include "logger/Logger.h"

unsigned int Renderer::clearBits;
std::unique_ptr<FrameBuffer> Renderer::ViewportFrameBuffer;

void Renderer::Initialize()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log(LogCategory::ERROR, "Failed to initialize GLAD", "Renderer::Initialize");
    }

    int width  = Window::GetWindowWidth();
    int height = Window::GetWindowHeight();
    
    ViewportFrameBuffer  = std::make_unique<FrameBuffer>();
    viewportRenderBuffer = std::make_shared<RenderBuffer>(width, height);
    viewportTexture      = std::make_shared<Texture>(TextureType::DIFFUSE);

    ViewportFrameBuffer->SetTextureAttachment(viewportTexture, COLOR_ATTACHMENT_0, TEXTURE_2D);
    ViewportFrameBuffer->SetRenderBufferAttachment(viewportRenderBuffer, DEPTH_STENCIL);

    ViewportFrameBuffer->CheckIsComplete();

    // For some reason OSX requires double the window dimensions
#if defined(_WIN32)
    SetViewport(0, 0, width, height);
#elif __APPLE__
    SetViewport(0, 0, 2 * width, 2 * height);
#endif
    
    clearBits = 0x0;
    
    SetPolygonMode(GL_FRONT, GL_FILL);
    AppendClearBits(GL_COLOR_BUFFER_BIT);
    SetDepthTestEnabled(true);
    SetStencilTestEnabled(false);

    Logger::Log(LogCategory::INFO, "Renderer initialization completed successfully", "Renderer::Initialize");
}

void Renderer::Clear()
{
    glClear(clearBits);
}

void Renderer::SetPolygonMode(int face, int mode)
{
    glPolygonMode(face, mode);
}

void Renderer::SetClearColor(const Color& c)
{
    glClearColor(c.r, c.g, c.b, c.a);
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::SetViewport(int bottomLeftX, int bottomLeftY, int width, int height)
{
    glViewport(bottomLeftX, bottomLeftY, width, height);

    viewportTexture->SetWidth(width);
    viewportTexture->SetHeight(height);
    viewportTexture->SetData(nullptr);
    
    viewportRenderBuffer->Resize(width, height);
}

void Renderer::SetDepthTestEnabled(bool state)
{
    if (state)
    {
        glEnable(GL_DEPTH_TEST);
        clearBits |= GL_DEPTH_BUFFER_BIT;
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        clearBits &= (GL_DEPTH_BUFFER_BIT ^ 0xFFFFFFFF);
    }
}

void Renderer::SetDepthTestMask(bool mask)
{
    glDepthMask(mask);
}

void Renderer::SetDepthTestFunc(unsigned int func)
{
    glDepthFunc(func);
}

void Renderer::SetStencilTestEnabled(bool state)
{
    if (state)
    {
        glEnable(GL_STENCIL_TEST);
        clearBits |= GL_STENCIL_BUFFER_BIT;
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
        clearBits &= (GL_STENCIL_BUFFER_BIT ^ 0xFFFFFFFF);
    }
}

void Renderer::SetStencilTestMask(unsigned int mask)
{
    glStencilMask(mask);
}

void Renderer::SetStencilTestFunc(unsigned int func, int reference, unsigned int mask)
{
    glStencilFunc(func, reference, mask);
}

void Renderer::SetStencilTestOp(unsigned int stencilFail, unsigned int depthFail, unsigned int depthPass)
{
    glStencilOp(stencilFail, depthFail, depthPass);
}

void Renderer::SetBlendingEnabled(bool state)
{
    state ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::SetBlendingFunc(unsigned int srcFactor, unsigned int dstFactor)
{
    glBlendFunc(srcFactor, dstFactor);
}

void Renderer::SetBlendingFuncSeparate(unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha)
{
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void Renderer::SetBlendingOp(unsigned int operation)
{
    glBlendEquation(operation);
}

void Renderer::SetFaceCullingEnabled(bool state)
{
    state ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void Renderer::SetFaceCullingMode(unsigned int mode)
{
    glCullFace(mode);
}

void Renderer::SetFaceFrontWindingOrder(unsigned int order)
{
    glFrontFace(order);
}

void Renderer::GetViewportResolution(int& outWidth, int& outHeight)
{
    GLint viewportData[4];
    glGetIntegerv(GL_VIEWPORT, viewportData);

    outWidth  = viewportData[2];
    outHeight = viewportData[3];
}
