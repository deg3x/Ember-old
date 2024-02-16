#include "engine_pch.h"
#include "ObjectPrimitive.h"

#include "core/World.h"
#include "core/Object.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include "core/Material.h"
#include "core/Renderer.h"
#include "core/components/Mesh.h"
#include "core/components/Transform.h"
#include "utils/PathBuilder.h"
#include "utils/ProceduralMesh.h"

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
    cubeMat->SetTexture("prefilterMap", Renderer::SkyboxPrefilteredMap);
    cubeMat->SetTexture("brdfMap", Renderer::SkyboxBRDFMap);
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
    planeMat->SetTexture("prefilterMap", Renderer::SkyboxPrefilteredMap);
    planeMat->SetTexture("brdfMap", Renderer::SkyboxBRDFMap);
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
    
    skyMat->SetTexture("skybox", Renderer::SkyboxCubeMapHDR);
    
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
    
    ProceduralMesh::GeneratePlane(1, 1000.0f, gridMesh);
    
    gridMesh->material           = gridMat;
    gridMesh->meshType           = MeshType::TRANSPARENT;
    gridMesh->cullingMode        = CullingMode::NONE;
    gridMesh->writeToDepthBuffer = false;

    // We push the grid lower to avoid Z-fighting
    editorGrid->transform->position = {0.0f, -0.01f, 0.0f};

    World::AddObject(editorGrid);

    return editorGrid;
}
