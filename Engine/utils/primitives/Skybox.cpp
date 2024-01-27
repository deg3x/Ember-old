#include "engine_pch.h"
#include "Skybox.h"

#include "core/components/Mesh.h"
#include "core/materials/MaterialSkybox.h"
#include "core/Object.h"
#include "core/World.h"
#include "core/textures/TextureCubemap.h"
#include "utils/procedural/Cube.h"

std::shared_ptr<Object> Skybox::Instantiate()
{
    const std::shared_ptr<Object> skybox = std::make_shared<Object>("Skybox");
    const std::shared_ptr<TextureCubemap> texture = std::make_shared<TextureCubemap>();
    const std::shared_ptr<MaterialSkybox> matSky = std::make_shared<MaterialSkybox>();
    
    matSky->SetCubemap(texture);

    const std::shared_ptr<Mesh> skyboxMesh = skybox->CreateComponent<Mesh>();
    Cube::GenerateCube(skyboxMesh);
    skyboxMesh->material = matSky;

    World::AddObject(skybox);

    return skybox;
}
