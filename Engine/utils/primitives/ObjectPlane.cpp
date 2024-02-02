#include "engine_pch.h"
#include "ObjectPlane.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/World.h"
#include "core/components/Mesh.h"
#include "core/Material.h"
#include "utils/PathBuilder.h"
#include "utils/procedural/Plane.h"

std::shared_ptr<Object> ObjectPlane::Instantiate()
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
    
    Plane::GeneratePlane(10, 10.0f, planeMesh);
    World::AddObject(planeObject);

    return planeObject;
}
