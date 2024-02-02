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
    const std::shared_ptr<Texture> sphereTex   = std::make_shared<Texture>("./Data/images/white.png", TextureType::DIFFUSE);

    sphereMat->SetTexture("diffuseTexture", sphereTex);
    //sphereMat->AddProperty("material.color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //sphereMat->AddProperty("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
    //sphereMat->AddProperty("material.shininess", 64.0f);

    sphereMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
    sphereMat->SetProperty("roughness", 0.1f);
    sphereMat->SetProperty("metallic", 0.9f);
    sphereMat->SetProperty("ambientOcclusion", 0.5f);

    sphereMesh->material = sphereMat;

    Sphere::GenerateSphere(32, 32, 0.5f, sphereMesh);
    World::AddObject(sphereObject);

    return sphereObject;
}
