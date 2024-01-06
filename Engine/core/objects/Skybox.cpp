#include "Skybox.h"

#include <glad/glad.h>

#include "core/Renderer.h"
#include "core/materials/Material.h"
#include "core/components/meshes/Cube.h"
#include "core/textures/TextureCubemap.h"
#include "core/materials/MaterialSkybox.h"

Skybox::Skybox()
{
    const std::shared_ptr<TextureCubemap> texture = std::make_shared<TextureCubemap>();
    const std::shared_ptr<MaterialSkybox> matSky = std::make_shared<MaterialSkybox>();

    matSky->SetCubemap(texture);

    CreateComponent<Cube>(matSky);
}

void Skybox::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    Renderer::SetDepthTestFunc(GL_LEQUAL);
    Object::Draw(camera, lights);
    Renderer::SetDepthTestFunc(GL_LESS);
}
