#include "engine_pch.h"
#include "ObjectPrimitive.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include "core/Material.h"
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

    cubeMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
    cubeMat->SetProperty("roughness", 0.3f);
    cubeMat->SetProperty("metallic", 0.0f);
    cubeMat->SetProperty("ambientOcclusion", 0.3f);

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

    planeMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
    planeMat->SetProperty("roughness", 0.3f);
    planeMat->SetProperty("metallic", 0.0f);
    planeMat->SetProperty("ambientOcclusion", 0.3f);
    
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

    const std::shared_ptr<Texture> sphereTexAlbedo    = std::make_shared<Texture>("./Data/images/metals/0071_color_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_0);
    const std::shared_ptr<Texture> sphereTexMetallic  = std::make_shared<Texture>("./Data/images/metals/0071_metallic_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_1);
    const std::shared_ptr<Texture> sphereTexRoughness = std::make_shared<Texture>("./Data/images/metals/0071_roughness_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_2);
    const std::shared_ptr<Texture> sphereTexAO        = std::make_shared<Texture>("./Data/images/metals/0071_ao_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_3);
    
    sphereMat->SetTexture("albedoMap", sphereTexAlbedo);
    sphereMat->SetTexture("metallicMap", sphereTexMetallic);
    sphereMat->SetTexture("roughnessMap", sphereTexRoughness);
    sphereMat->SetTexture("ambientOcclusionMap", sphereTexAO);

    sphereMat->SetProperty("hasMapAlbedo", true);
    sphereMat->SetProperty("hasMapMetallic", true);
    sphereMat->SetProperty("hasMapRoughness", true);
    sphereMat->SetProperty("hasMapAmbientOcclusion", true);

    sphereMesh->material = sphereMat;

    ProceduralMesh::GenerateSphere(32, 32, 0.5f, sphereMesh);
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
    const std::shared_ptr<Texture> skyTex   = std::make_shared<Texture>("./Data/images/skybox/cubemap_clouds_", TextureType::CUBEMAP, TextureUnit::TEX_0);
    
    skyMat->SetTexture("skybox", skyTex);

    const std::shared_ptr<Mesh> skyboxMesh = skyObject->CreateComponent<Mesh>();
    ProceduralMesh::GenerateCube(skyboxMesh);
    
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
