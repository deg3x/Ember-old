#include "engine_pch.h"
#include "ObjectSphere.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/World.h"
#include "core/components/Mesh.h"
#include "core/Material.h"
#include "utils/PathBuilder.h"
#include "utils/procedural/Sphere.h"

std::shared_ptr<Object> ObjectSphere::Instantiate()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");
    
    const std::shared_ptr<Object> sphereObject = std::make_shared<Object>("Sphere");
    const std::shared_ptr<Mesh> sphereMesh     = sphereObject->CreateComponent<Mesh>();
    const std::shared_ptr<Shader> sphereShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> sphereMat  = std::make_shared<Material>(sphereShader);

    sphereMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
    sphereMat->SetProperty("roughness", 0.1f);
    sphereMat->SetProperty("metallic", 0.9f);
    sphereMat->SetProperty("ambientOcclusion", 0.5f);

    const std::shared_ptr<Texture> sphereTexAlbedo    = std::make_shared<Texture>("./Data/images/metals/0071_color_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_0);
    const std::shared_ptr<Texture> sphereTexMetallic  = std::make_shared<Texture>("./Data/images/metals/0071_metallic_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_1);
    const std::shared_ptr<Texture> sphereTexRoughness = std::make_shared<Texture>("./Data/images/metals/0071_roughness_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_2);
    const std::shared_ptr<Texture> sphereTexAO        = std::make_shared<Texture>("./Data/images/metals/0071_ao_4k.jpg", TextureType::DIFFUSE, TextureUnit::TEX_3);
    
    //sphereMat->SetTexture("albedoMap", "./Data/images/metals/0071_color_4k.jpg", TextureType::DIFFUSE);
    sphereMat->SetTexture("albedoMap", sphereTexAlbedo);
    sphereMat->SetTexture("metallicMap", sphereTexMetallic);
    sphereMat->SetTexture("roughnessMap", sphereTexRoughness);
    sphereMat->SetTexture("ambientOcclusionMap", sphereTexAO);

    sphereMat->SetProperty("hasMapAlbedo", true);
    sphereMat->SetProperty("hasMapMetallic", true);
    sphereMat->SetProperty("hasMapRoughness", true);
    sphereMat->SetProperty("hasMapAmbientOcclusion", true);

    sphereMesh->material = sphereMat;

    Sphere::GenerateSphere(32, 32, 0.5f, sphereMesh);
    World::AddObject(sphereObject);

    return sphereObject;
}
