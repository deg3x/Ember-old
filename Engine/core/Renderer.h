#pragma once

#include "Definitions.h"
#include "Texture.h"

class Mesh;
class Light;
class Texture;
class RenderBuffer;
class FrameBuffer;
struct Color;

class ENGINE_API Renderer
{
public:
    static std::unique_ptr<FrameBuffer> ViewportFrameBuffer;
    static std::shared_ptr<Texture> SkyboxCubeMapHDR;
    static std::shared_ptr<Texture> SkyboxIrradianceMap;
    static std::shared_ptr<Texture> SkyboxPrefilteredMap;
    static std::shared_ptr<Texture> SkyboxBRDFMap;
    
private:
    inline static std::shared_ptr<RenderBuffer> viewportRenderBuffer;
    inline static std::shared_ptr<Texture> viewportTexture;
    inline static std::vector<std::shared_ptr<Mesh>> renderQueue;
    inline static std::vector<std::shared_ptr<Light>> lights;
    
    static unsigned int clearBits;
    
public:
    Renderer() = delete;
    
    static void Initialize();
    static void Tick();
    static void Clear();

    static void DrawMesh(const std::shared_ptr<Mesh>& mesh);

    static void RenderQueueAppend(const std::shared_ptr<Mesh>& mesh);
    static void RenderQueuePrepend(const std::shared_ptr<Mesh>& mesh);
    static void RenderQueueRemove(const std::shared_ptr<Mesh>& mesh);

    static void LightsAppend(const std::shared_ptr<Light>& light);
    static void LightsRemove(const std::shared_ptr<Light>& light);
    
    static void SetPolygonMode(int face, int mode);
    static void SetClearColor(const Color& c);
    static void SetClearColor(float r, float g, float b, float a);
    static void SetViewport(int bottomLeftX, int bottomLeftY, int width, int height);
    static void SetDepthTestEnabled(bool state = true);
    static void SetDepthTestMask(bool mask);
    static void SetDepthTestFunc(unsigned int func);
    static void SetStencilTestEnabled(bool state = true);
    static void SetStencilTestMask(unsigned int mask);
    static void SetStencilTestFunc(unsigned int func, int reference, unsigned int mask);
    static void SetStencilTestOp(unsigned int stencilFail, unsigned int depthFail, unsigned int depthPass);
    static void SetBlendingEnabled(bool state = false);
    static void SetBlendingFunc(unsigned int srcFactor, unsigned int dstFactor);
    static void SetBlendingFuncSeparate(unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha);
    static void SetBlendingOp(unsigned int operation);
    static void SetFaceCullingEnabled(bool state = true);
    static void SetFaceCullingMode(unsigned int mode);
    static void SetFaceFrontWindingOrder(unsigned int order);

    static void GetViewportResolution(int& outWidth, int& outHeight);

    static void AppendClearBits(unsigned int bitsToAppend)
    {
        clearBits |= bitsToAppend;
    }

    static void RemoveClearBits(unsigned int bitsToRemove)
    {
        clearBits &= (bitsToRemove ^ 0xFFFFFFFF);
    }

    static unsigned int GetViewportTextureID()
    {
        return viewportTexture->GetID();
    }
    
    static unsigned int GetClearBits()
    {
        return clearBits;
    }

private:
    static void InitializeSkyboxIBL();
};
