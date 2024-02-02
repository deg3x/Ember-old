#include "engine_pch.h"
#include "Skybox.h"

#include "core/components/Mesh.h"
#include "core/Material.h"
#include "core/Object.h"
#include "core/Shader.h"
#include "core/World.h"
#include "core/Texture.h"
#include "utils/PathBuilder.h"
#include "utils/procedural/Cube.h"

std::shared_ptr<Object> Skybox::Instantiate()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexSkybox.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentSkybox.glsl");
    
    const std::shared_ptr<Object> skyObject = std::make_shared<Object>("Skybox");
    const std::shared_ptr<Shader> skyShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> skyMat  = std::make_shared<Material>(skyShader);
    const std::shared_ptr<Texture> skyTex   = std::make_shared<Texture>("./Data/images/skybox/cubemap_clouds_", TextureType::CUBEMAP, TextureUnit::TEX_0);
    
    skyMat->SetTexture("skybox", skyTex);

    const std::shared_ptr<Mesh> skyboxMesh = skyObject->CreateComponent<Mesh>();
    Cube::GenerateCube(skyboxMesh);
    
    skyboxMesh->material    = skyMat;
    skyboxMesh->cullingMode = CullingMode::FRONT;
    skyboxMesh->depthTest   = DepthTestMode::LEQUAL;

    World::AddObject(skyObject);

    return skyObject;
}
