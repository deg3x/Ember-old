#include "engine_pch.h"
#include "ObjectPrimitive.h"

#include <glad/glad.h>

#include "core/FrameBuffer.h"
#include "core/Object.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include "core/Material.h"
#include "core/RenderBuffer.h"
#include "core/Renderer.h"
#include "core/components/Mesh.h"
#include "core/components/Transform.h"
#include "utils/PathBuilder.h"
#include "utils/ProceduralMesh.h"

namespace
{
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions       // normals          // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
}

std::shared_ptr<Object> ObjectPrimitive::InstantiateCube()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");
    
    const std::shared_ptr<Object> cubeObject = std::make_shared<Object>("Cube");
    const std::shared_ptr<Mesh> cubeMesh     = cubeObject->CreateComponent<Mesh>();
    const std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> cubeMat  = std::make_shared<Material>(cubeShader);
    const std::shared_ptr<Texture> cubeTex   = std::make_shared<Texture>("./Data/images/white.png", TextureType::DIFFUSE);

    const std::shared_ptr<Texture> texAlbedo    = std::make_shared<Texture>("./Data/images/marble/1K/cracks_color.jpg", TextureType::DIFFUSE, TextureUnit::TEX_0);
    const std::shared_ptr<Texture> texNormal    = std::make_shared<Texture>("./Data/images/marble/1K/cracks_normal.png", TextureType::DIFFUSE, TextureUnit::TEX_1);
    const std::shared_ptr<Texture> texRoughness = std::make_shared<Texture>("./Data/images/marble/1K/cracks_roughness.jpg", TextureType::DIFFUSE, TextureUnit::TEX_2);
    const std::shared_ptr<Texture> texAO        = std::make_shared<Texture>("./Data/images/marble/1K/cracks_ao.jpg", TextureType::DIFFUSE, TextureUnit::TEX_3);

    cubeMat->SetProperty("metallic", 0.0f);
    
    cubeMat->SetTexture("albedoMap", texAlbedo);
    cubeMat->SetTexture("normalMap", texNormal);
    cubeMat->SetTexture("roughnessMap", texRoughness);
    cubeMat->SetTexture("ambientOcclusionMap", texAO);

    cubeMat->SetProperty("hasMapAlbedo", true);
    cubeMat->SetProperty("hasMapNormal", true);
    cubeMat->SetProperty("hasMapRoughness", true);
    cubeMat->SetProperty("hasMapAmbientOcclusion", true);
    
    cubeMat->SetTexture("irradianceMap", Renderer::SkyboxIrradianceMap);
    cubeMat->SetProperty("hasImageBasedLighting", true);

    cubeMesh->material = cubeMat;

    ProceduralMesh::GenerateCube(cubeMesh);
    World::AddObject(cubeObject);

    return cubeObject;
}

std::shared_ptr<Object> ObjectPrimitive::InstantiatePlane()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");
    
    const std::shared_ptr<Object> planeObject = std::make_shared<Object>("Plane");
    const std::shared_ptr<Mesh> planeMesh     = planeObject->CreateComponent<Mesh>();
    const std::shared_ptr<Shader> planeShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> planeMat  = std::make_shared<Material>(planeShader);
    const std::shared_ptr<Texture> planeTex   = std::make_shared<Texture>("./Data/images/white.png", TextureType::DIFFUSE);

    const std::shared_ptr<Texture> texAlbedo    = std::make_shared<Texture>("./Data/images/marble/1K/cracks_color.jpg", TextureType::DIFFUSE, TextureUnit::TEX_0);
    const std::shared_ptr<Texture> texNormal    = std::make_shared<Texture>("./Data/images/marble/1K/cracks_normal.png", TextureType::DIFFUSE, TextureUnit::TEX_1);
    const std::shared_ptr<Texture> texRoughness = std::make_shared<Texture>("./Data/images/marble/1K/cracks_roughness.jpg", TextureType::DIFFUSE, TextureUnit::TEX_2);
    const std::shared_ptr<Texture> texAO        = std::make_shared<Texture>("./Data/images/marble/1K/cracks_ao.jpg", TextureType::DIFFUSE, TextureUnit::TEX_3);

    planeMat->SetProperty("metallic", 0.0f);
    
    planeMat->SetTexture("albedoMap", texAlbedo);
    planeMat->SetTexture("normalMap", texNormal);
    planeMat->SetTexture("roughnessMap", texRoughness);
    planeMat->SetTexture("ambientOcclusionMap", texAO);

    planeMat->SetProperty("hasMapAlbedo", true);
    planeMat->SetProperty("hasMapNormal", true);
    planeMat->SetProperty("hasMapRoughness", true);
    planeMat->SetProperty("hasMapAmbientOcclusion", true);

    planeMat->SetTexture("irradianceMap", Renderer::SkyboxIrradianceMap);
    planeMat->SetProperty("hasImageBasedLighting", true);
    
    planeMesh->material = planeMat;
    
    ProceduralMesh::GeneratePlane(10, 10.0f, planeMesh);
    World::AddObject(planeObject);

    return planeObject;
}

std::shared_ptr<Object> ObjectPrimitive::InstantiateSphere()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");
    
    const std::shared_ptr<Object> sphereObject = std::make_shared<Object>("Sphere");
    const std::shared_ptr<Mesh> sphereMesh     = sphereObject->CreateComponent<Mesh>();
    const std::shared_ptr<Shader> sphereShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> sphereMat  = std::make_shared<Material>(sphereShader);

    const std::shared_ptr<Texture> texAlbedo    = std::make_shared<Texture>("./Data/images/metals/1K/copper_color.jpg", TextureType::DIFFUSE, TextureUnit::TEX_0);
    const std::shared_ptr<Texture> texNormal    = std::make_shared<Texture>("./Data/images/metals/1K/copper_normal.png", TextureType::DIFFUSE, TextureUnit::TEX_1);
    const std::shared_ptr<Texture> texRoughness = std::make_shared<Texture>("./Data/images/metals/1K/copper_roughness.jpg", TextureType::DIFFUSE, TextureUnit::TEX_2);
    const std::shared_ptr<Texture> texAO        = std::make_shared<Texture>("./Data/images/metals/1K/copper_ao.jpg", TextureType::DIFFUSE, TextureUnit::TEX_3);
    const std::shared_ptr<Texture> texMetallic  = std::make_shared<Texture>("./Data/images/metals/1K/copper_metallic.jpg", TextureType::DIFFUSE, TextureUnit::TEX_4);
    
    sphereMat->SetTexture("metallicMap", texMetallic);
    sphereMat->SetTexture("albedoMap", texAlbedo);
    sphereMat->SetTexture("normalMap", texNormal);
    sphereMat->SetTexture("roughnessMap", texRoughness);
    sphereMat->SetTexture("ambientOcclusionMap", texAO);

    sphereMat->SetProperty("hasMapMetallic", true);
    sphereMat->SetProperty("hasMapAlbedo", true);
    sphereMat->SetProperty("hasMapNormal", true);
    sphereMat->SetProperty("hasMapRoughness", true);
    sphereMat->SetProperty("hasMapAmbientOcclusion", true);

    sphereMat->SetTexture("irradianceMap", Renderer::SkyboxIrradianceMap);
    sphereMat->SetTexture("prefilterMap", Renderer::SkyboxPrefilteredMap);
    sphereMat->SetTexture("brdfMap", Renderer::SkyboxBRDFMap);
    sphereMat->SetProperty("hasImageBasedLighting", true);

    sphereMesh->material = sphereMat;

    ProceduralMesh::GenerateSphere(64, 64, 0.5f, sphereMesh);
    World::AddObject(sphereObject);

    return sphereObject;
}

std::shared_ptr<Object> ObjectPrimitive::InstantiateSkybox()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexSkybox.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentSkybox.glsl");
    
    const std::shared_ptr<Object> skyObject = std::make_shared<Object>("Skybox");
    const std::shared_ptr<Shader> skyShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> skyMat  = std::make_shared<Material>(skyShader);
    
    const std::shared_ptr<Mesh> skyboxMesh = skyObject->CreateComponent<Mesh>();
    ProceduralMesh::GenerateCube(skyboxMesh);

    const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    const glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
     };

    const std::string vertHDR = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragHDR = PathBuilder::GetPath("./Engine/shaders/fragmentHDRI.glsl");
    const std::shared_ptr<Shader> hdrShader = std::make_shared<Shader>(vertHDR.c_str(), fragHDR.c_str());
    const std::shared_ptr<Texture> hdrTex   = std::make_shared<Texture>("./Data/images/HDR/cloudy_dusky_sky_4k.hdr", TextureType::HDR, TEX_0, RGB16F, RGB, FLOAT);

    constexpr int cubeMapResolution = 2048;
    
    const std::shared_ptr<FrameBuffer> fb  = std::make_shared<FrameBuffer>();
    const std::shared_ptr<RenderBuffer> rb = std::make_shared<RenderBuffer>(cubeMapResolution, cubeMapResolution, TextureFormat::DEPTH24);
    fb->SetRenderBufferAttachment(rb, RenderAttachment::DEPTH);

    const std::shared_ptr<Texture> cubeMap = std::make_shared<Texture>(TextureType::CUBE_MAP, TEX_0, RGB16F, RGB, FLOAT, cubeMapResolution, cubeMapResolution);
    cubeMap->SetParameter(TEXTURE_CUBE_MAP, TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR);
    
    const std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    ProceduralMesh::GenerateCube(mesh);

    hdrTex->Bind();
    hdrShader->Use();
    hdrShader->SetInt("equirectMap", hdrTex->GetUnit() - TEX_0);
    hdrShader->SetMatrix4x4("projection", captureProjection);

    glViewport(0, 0, cubeMapResolution, cubeMapResolution);
    fb->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        hdrShader->SetMatrix4x4("view", captureViews[i]);
        fb->SetTextureAttachment(cubeMap, COLOR_ATTACHMENT_0, (TextureTarget)(TEXTURE_CUBE_MAP_POSITIVE_X + i));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    }
    fb->Unbind();

    cubeMap->GenerateMipmap(TEXTURE_CUBE_MAP);

    const std::string vertIrr = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragIrr = PathBuilder::GetPath("./Engine/shaders/fragmentIrradianceMap.glsl");
    const std::shared_ptr<Shader> irrShader = std::make_shared<Shader>(vertIrr.c_str(), fragIrr.c_str());
    
    constexpr int irrMapResolution = 32;
    const std::shared_ptr<Texture> irradianceMap = std::make_shared<Texture>(TextureType::CUBE_MAP, TEX_31, RGB16F, RGB, FLOAT, irrMapResolution, irrMapResolution);

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

        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    }
    fb->Unbind();

    const std::string vertPrefilter = PathBuilder::GetPath("./Engine/shaders/vertexHDRI.glsl");
    const std::string fragPrefilter = PathBuilder::GetPath("./Engine/shaders/fragmentFilteredEnvMap.glsl");
    const std::shared_ptr<Shader> prefilterShader = std::make_shared<Shader>(vertPrefilter.c_str(), fragPrefilter.c_str());
    
    constexpr int filtEnvMapResolution = 256;
    const std::shared_ptr<Texture> filteredEnvMap = std::make_shared<Texture>(TextureType::CUBE_MAP, TEX_30, RGB16F, RGB, FLOAT, filtEnvMapResolution, filtEnvMapResolution);
    filteredEnvMap->SetParameter(TEXTURE_CUBE_MAP, TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR);
    filteredEnvMap->GenerateMipmap(TEXTURE_CUBE_MAP);

    cubeMap->Bind();
    prefilterShader->Use();
    prefilterShader->SetInt("environmentMap", cubeMap->GetUnit() - TEX_0);
    prefilterShader->SetMatrix4x4("projection", captureProjection);
    
    fb->Bind();
    constexpr int maxMipLevels = 5;
    for (int level = 0; level < maxMipLevels; level++)
    {
        int mipResolution = static_cast<int>(static_cast<float>(filtEnvMapResolution) * std::pow(0.5, level));

        rb->Resize(mipResolution, mipResolution);
        glViewport(0, 0, mipResolution, mipResolution);

        float roughness = static_cast<float>(level) / static_cast<float>(maxMipLevels - 1);
        prefilterShader->SetFloat("roughness", roughness);
        for (int i = 0; i < 6; i++)
        {
            prefilterShader->SetMatrix4x4("view", captureViews[i]);
            fb->SetTextureAttachment(filteredEnvMap, COLOR_ATTACHMENT_0, (TextureTarget)(TEXTURE_CUBE_MAP_POSITIVE_X + i), level);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(mesh->VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
        }
    }
    fb->Unbind();

    const std::string vertBRDF = PathBuilder::GetPath("./Engine/shaders/vertexScreen.glsl");
    const std::string fragBRDF = PathBuilder::GetPath("./Engine/shaders/fragmentIntegrationBrdf.glsl");
    const std::shared_ptr<Shader> brdfShader = std::make_shared<Shader>(vertBRDF.c_str(), fragBRDF.c_str());
    
    constexpr int brdfResolution = 512;
    const std::shared_ptr<Texture> integratedBRDFMap = std::make_shared<Texture>(TextureType::DIFFUSE, TEX_29, RGB16F, RG, FLOAT, brdfResolution, brdfResolution);
    integratedBRDFMap->SetParameter(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
    integratedBRDFMap->SetParameter(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);

    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);

    fb->Bind();
    rb->Resize(brdfResolution, brdfResolution);
    fb->SetTextureAttachment(integratedBRDFMap, COLOR_ATTACHMENT_0, TEXTURE_2D);

    glViewport(0, 0, brdfResolution, brdfResolution);
    brdfShader->Use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // We using an anonymous NS function for now. Add quad mesh or fix plane for resolution 1.
    renderQuad();
    fb->Unbind();

    Renderer::SkyboxBRDFMap        = integratedBRDFMap;
    Renderer::SkyboxCubeMapHDR     = cubeMap;
    Renderer::SkyboxIrradianceMap  = irradianceMap;
    Renderer::SkyboxPrefilteredMap = filteredEnvMap;
    
    skyMat->SetTexture("skybox", cubeMap);
    
    skyboxMesh->material    = skyMat;
    skyboxMesh->cullingMode = CullingMode::FRONT;
    skyboxMesh->depthTest   = DepthTestMode::LEQUAL;

    World::AddObject(skyObject);

    return skyObject;
}

std::shared_ptr<Object> ObjectPrimitive::InstantiateEditorGrid()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexEditorGrid.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentEditorGrid.glsl");
    
    const std::shared_ptr<Object> editorGrid = std::make_shared<Object>("Editor Grid");
    const std::shared_ptr<Shader> gridShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> gridMat  = std::make_shared<Material>(gridShader);
    const std::shared_ptr<Mesh> gridMesh     = editorGrid->CreateComponent<Mesh>();
    
    ProceduralMesh::GeneratePlane(10, 100.0f, gridMesh);
    
    gridMesh->material           = gridMat;
    gridMesh->meshType           = MeshType::TRANSPARENT;
    gridMesh->cullingMode        = CullingMode::NONE;
    gridMesh->writeToDepthBuffer = false;

    // We push the grid lower to avoid Z-fighting
    editorGrid->transform->position = {0.0f, -0.01f, 0.0f};

    World::AddObject(editorGrid);

    return editorGrid;
}
