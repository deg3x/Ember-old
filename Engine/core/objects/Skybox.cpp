#include "Skybox.h"

#include <glad/glad.h>

#include "../Texture.h"
#include "../materials/Material.h"
#include "../Renderer.h"
#include "../components/meshes/Cube.h"
#include "../materials/MaterialSkybox.h"

Skybox::Skybox()
{
    const std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    const std::shared_ptr<MaterialSkybox> matSky = std::make_shared<MaterialSkybox>();

    matSky->SetCubemap(texture);

    CreateComponent<Cube>(matSky);
}

Skybox::~Skybox()
{
}

void Skybox::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    Renderer::SetDepthTestFunc(GL_LEQUAL);
    Object::Draw(camera, lights);
    Renderer::SetDepthTestFunc(GL_LESS);
}
