#include "engine_pch.h"
#include "ObjectCube.h"

#include "core/Object.h"
#include "core/Shader.h"
#include "core/World.h"
#include "core/components/Mesh.h"
#include "core/materials/Material.h"
#include "utils/PathBuilder.h"
#include "utils/procedural/Cube.h"

std::shared_ptr<Object> ObjectCube::Instantiate()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexPhong.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPhong.glsl");
    
    const std::shared_ptr<Object> cubeObject = std::make_shared<Object>("Cube");
    const std::shared_ptr<Mesh> cubeMesh     = cubeObject->CreateComponent<Mesh>();
    const std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> cubeMat  = std::make_shared<Material>(cubeShader);
    const std::shared_ptr<Texture> cubeTex   = std::make_shared<Texture>("./Data/images/white.png", TextureType::DIFFUSE);

    cubeMat->AddTexture("diffuseTexture", cubeTex);
    cubeMat->AddProperty("material.diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    cubeMat->AddProperty("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
    cubeMat->AddProperty("material.shininess", 64.0f);

    cubeMesh->material = cubeMat;

    Cube::GenerateCube(cubeMesh);
    World::AddObject(cubeObject);

    return cubeObject;
}
