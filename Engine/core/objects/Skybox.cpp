#include "engine_pch.h"
#include "Skybox.h"

#include "glad/glad.h"

#include "core/Renderer.h"
#include "core/components/Mesh.h"
#include "core/textures/TextureCubemap.h"
#include "core/materials/MaterialSkybox.h"
#include "utils/procedural/Cube.h"

Skybox::Skybox(const char* objName) : Object(objName)
{
    const std::shared_ptr<TextureCubemap> texture = std::make_shared<TextureCubemap>();
    const std::shared_ptr<MaterialSkybox> matSky = std::make_shared<MaterialSkybox>();
    
    matSky->SetCubemap(texture);

    const std::shared_ptr<Mesh> skyboxMesh = CreateComponent<Mesh>();
    Cube::GenerateCube(skyboxMesh);
    skyboxMesh->material = matSky;
}

void Skybox::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    Renderer::SetDepthTestFunc(GL_LEQUAL);
    Object::Draw(camera, lights);
    Renderer::SetDepthTestFunc(GL_LESS);
}
