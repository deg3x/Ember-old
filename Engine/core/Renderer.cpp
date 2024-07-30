#include "engine_pch.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "FrameBuffer.h"
#include "Material.h"
#include "Object.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "components/Camera.h"
#include "components/Light.h"
#include "components/Mesh.h"

#include "window/Window.h"
#include "utils/Types.h"
#include "logger/Logger.h"
#include "utils/PathBuilder.h"
#include "utils/ProceduralMesh.h"

unsigned int Renderer::clearBits;
std::unique_ptr<FrameBuffer> Renderer::ViewportFrameBuffer;
std::shared_ptr<Texture> Renderer::SkyboxCubeMapHDR;
std::shared_ptr<Texture> Renderer::SkyboxIrradianceMap;
std::shared_ptr<Texture> Renderer::SkyboxPrefilteredMap;
std::shared_ptr<Texture> Renderer::SkyboxBRDFMap;

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

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    InitializeSkyboxIBL();

    Logger::Log(LogCategory::INFO, "Renderer initialization completed successfully", "Renderer::Initialize");
}

void Renderer::Tick()
{
    if (Camera::ActiveCamera == nullptr)
    {
        return;
    }
    
    for (const std::shared_ptr<Mesh>& mesh : renderQueue)
    {
        DrawMesh(mesh);
    }
}

void Renderer::Clear()
{
    glClear(clearBits);
}

void Renderer::DrawMesh(const std::shared_ptr<Mesh>& mesh)
{
    if (!mesh->GetOwner()->isActive)
    {
        return;
    }

    if (mesh->material == nullptr)
    {
        Logger::Log(LogCategory::ERROR, "Mesh has no material: " + mesh->GetOwner()->name, "Mesh::Draw");
        return;
    }

    mesh->material->Use();

    int lightIdxDir   = 0;
    int lightIdxPoint = 0;
    int lightIdxSpot  = 0;
    for (const std::shared_ptr<Light>& light : lights)
    {
        const Object* lightObject = light->GetOwner();
        if (lightObject != nullptr)
        {
            if (!lightObject->isActive)
            {
                continue;
            }
        }
        
        switch (light->type)
        {
        case LightType::DIRECTIONAL:
            light->SetShaderProperties(*mesh->material->GetShader(), lightIdxDir);
            lightIdxDir++;
            break;
        case LightType::POINT:
            light->SetShaderProperties(*mesh->material->GetShader(), lightIdxPoint);
            lightIdxPoint++;
            break;
        case LightType::SPOTLIGHT:
            light->SetShaderProperties(*mesh->material->GetShader(), lightIdxSpot);
            lightIdxSpot++;
            break;
        }
    }
    mesh->material->SetInt("activeLightsDir", lightIdxDir);
    mesh->material->SetInt("activeLightsPoint", lightIdxPoint);
    mesh->material->SetInt("activeLightsSpot", lightIdxSpot);
    
    mesh->material->SetupShaderVariables(*mesh->GetOwner()->transform, *Camera::ActiveCamera);

    mesh->SetupDepthTestMode();
    mesh->SetupCullingMode();
    mesh->SetupPolygonMode();
    
    glBindVertexArray(mesh->GetVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

    mesh->ResetRendererState();
}

void Renderer::RenderQueueAppend(const std::shared_ptr<Mesh>& mesh)
{
    renderQueue.emplace_back(mesh);
}

void Renderer::RenderQueuePrepend(const std::shared_ptr<Mesh>& mesh)
{
    renderQueue.insert(renderQueue.begin(), mesh);
}

void Renderer::RenderQueueRemove(const std::shared_ptr<Mesh>& mesh)
{
    const auto iterator = std::ranges::find(renderQueue, mesh);

    if (iterator != renderQueue.end())
    {
        renderQueue.erase(iterator);
    }
}

void Renderer::LightsAppend(const std::shared_ptr<Light>& light)
{
    lights.emplace_back(light);
}

void Renderer::LightsRemove(const std::shared_ptr<Light>& light)
{
    const auto iterator = std::ranges::find(lights, light);

    if (iterator != lights.end())
    {
        lights.erase(iterator);
    }
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

void Renderer::InitializeSkyboxIBL()
{
    //const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    //const glm::mat4 captureViews[] = {
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    //    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    // };
    const Matrix4x4 captureProjection = Matrix4x4::Perspective(HALF_PI, 1.0f, 0.1f, 10.0f);
    const Matrix4x4 captureViews[] = {
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }),
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, {-1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }),
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }),
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f, -1.0f }),
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }),
        Matrix4x4::LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, -1.0f,  0.0f })
    };

    const std::string vertHDR  = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragHDR  = PathBuilder::GetPath("./Engine/shaders/fragmentHDRI.glsl");
    const std::string vertIrr  = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragIrr  = PathBuilder::GetPath("./Engine/shaders/fragmentIrradianceMap.glsl");
    const std::string vertPref = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragPref = PathBuilder::GetPath("./Engine/shaders/fragmentFilteredEnvMap.glsl");
    const std::string vertBRDF = PathBuilder::GetPath("./Engine/shaders/vertexScreen.glsl");
    const std::string fragBRDF = PathBuilder::GetPath("./Engine/shaders/fragmentIntegrationBrdf.glsl");
    
    const std::shared_ptr<Shader> hdrShader  = std::make_shared<Shader>(vertHDR.c_str(), fragHDR.c_str());
    const std::shared_ptr<Shader> irrShader  = std::make_shared<Shader>(vertIrr.c_str(), fragIrr.c_str());
    const std::shared_ptr<Shader> filtShader = std::make_shared<Shader>(vertPref.c_str(), fragPref.c_str());
    const std::shared_ptr<Shader> brdfShader = std::make_shared<Shader>(vertBRDF.c_str(), fragBRDF.c_str());

    constexpr int cubeMapResolution = 1024;
    constexpr int irrMapResolution  = 32;
    constexpr int filtMapResolution = 256;
    constexpr int brdfResolution    = 512;
    
    const std::shared_ptr<Texture> skyboxTexHDR  = std::make_shared<Texture>("./Data/images/HDR/cloudy_dusky_sky_4k.hdr", TextureType::HDR, TEX_0, RGB16F, RGB, FLOAT);
    const std::shared_ptr<Texture> cubeMap       = std::make_shared<Texture>(TextureType::CUBE_MAP, nullptr, TEX_0, RGB16F, RGB, FLOAT, cubeMapResolution, cubeMapResolution);
    const std::shared_ptr<Texture> irradianceMap = std::make_shared<Texture>(TextureType::CUBE_MAP, nullptr, TEX_31, RGB16F, RGB, FLOAT, irrMapResolution, irrMapResolution);
    const std::shared_ptr<Texture> filteredMap   = std::make_shared<Texture>(TextureType::CUBE_MAP, nullptr, TEX_30, RGB16F, RGB, FLOAT, filtMapResolution, filtMapResolution);
    const std::shared_ptr<Texture> brdfLutMap    = std::make_shared<Texture>(TextureType::DIFFUSE, nullptr, TEX_29, RGB16F, RG, FLOAT, brdfResolution, brdfResolution);

    cubeMap->SetParameter(TEXTURE_CUBE_MAP, TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR);
    
    filteredMap->SetParameter(TEXTURE_CUBE_MAP, TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR);
    filteredMap->GenerateMipmap(TEXTURE_CUBE_MAP);
    
    brdfLutMap->SetParameter(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
    brdfLutMap->SetParameter(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);

    const std::shared_ptr<FrameBuffer> fb  = std::make_shared<FrameBuffer>();
    const std::shared_ptr<RenderBuffer> rb = std::make_shared<RenderBuffer>(cubeMapResolution, cubeMapResolution, DEPTH24);
    fb->SetRenderBufferAttachment(rb, DEPTH);

    const std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>();
    const std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>();
    ProceduralMesh::GenerateCube(cubeMesh);
    ProceduralMesh::GenerateQuad(quadMesh);

    ////// Map equirectangular texture to cube
    skyboxTexHDR->Bind();
    hdrShader->Use();
    hdrShader->SetInt("equirectMap", skyboxTexHDR->GetUnit() - TEX_0);
    hdrShader->SetMatrix4x4("projection", captureProjection);

    glViewport(0, 0, cubeMapResolution, cubeMapResolution);
    fb->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        hdrShader->SetMatrix4x4("view", captureViews[i]);
        fb->SetTextureAttachment(cubeMap, COLOR_ATTACHMENT_0, (TextureTarget)(TEXTURE_CUBE_MAP_POSITIVE_X + i));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cubeMesh->GetVAO());
        glDrawElements(GL_TRIANGLES, (GLsizei)cubeMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    }
    fb->Unbind();
    //////

    cubeMap->GenerateMipmap(TEXTURE_CUBE_MAP); // Generate mipmaps after equirect mapping

    ////// Capture irradiance map for diffuse IBL
    rb->Resize(irrMapResolution, irrMapResolution);

    cubeMap->Bind();
    irrShader->Use();
    irrShader->SetInt("environmentMap", cubeMap->GetUnit() - TEX_0);
    irrShader->SetMatrix4x4("projection", captureProjection);

    glViewport(0, 0, irrMapResolution, irrMapResolution);
    fb->Bind();
    for (int i = 0; i < 6; i++)
    {
        irrShader->SetMatrix4x4("view", captureViews[i]);
        fb->SetTextureAttachment(irradianceMap, COLOR_ATTACHMENT_0, (TextureTarget)(TEXTURE_CUBE_MAP_POSITIVE_X + i));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cubeMesh->GetVAO());
        glDrawElements(GL_TRIANGLES, (GLsizei)cubeMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    }
    fb->Unbind();
    //////
    
    ////// Prefilter environment map for specular IBL
    cubeMap->Bind();
    filtShader->Use();
    filtShader->SetInt("environmentMap", cubeMap->GetUnit() - TEX_0);
    filtShader->SetMatrix4x4("projection", captureProjection);
    
    fb->Bind();
    constexpr int maxMipLevels = 5;
    for (int level = 0; level < maxMipLevels; level++)
    {
        int mipResolution = static_cast<int>(static_cast<float>(filtMapResolution) * std::pow(0.5, level));

        rb->Resize(mipResolution, mipResolution);
        glViewport(0, 0, mipResolution, mipResolution);

        float roughness = static_cast<float>(level) / static_cast<float>(maxMipLevels - 1);
        filtShader->SetFloat("roughness", roughness);
        for (int i = 0; i < 6; i++)
        {
            filtShader->SetMatrix4x4("view", captureViews[i]);
            fb->SetTextureAttachment(filteredMap, COLOR_ATTACHMENT_0, (TextureTarget)(TEXTURE_CUBE_MAP_POSITIVE_X + i), level);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(cubeMesh->GetVAO());
            glDrawElements(GL_TRIANGLES, (GLsizei)cubeMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
        }
    }
    fb->Unbind();
    //////

    ////// Generate BRDF LUT for use in specular IBL
    fb->Bind();
    rb->Resize(brdfResolution, brdfResolution);
    fb->SetTextureAttachment(brdfLutMap, COLOR_ATTACHMENT_0, TEXTURE_2D);

    glViewport(0, 0, brdfResolution, brdfResolution);
    brdfShader->Use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(quadMesh->GetVAO());
    glDrawElements(GL_TRIANGLES, (GLsizei)quadMesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    fb->Unbind();
    //////

    SkyboxBRDFMap        = brdfLutMap;
    SkyboxCubeMapHDR     = cubeMap;
    SkyboxIrradianceMap  = irradianceMap;
    SkyboxPrefilteredMap = filteredMap;
}
