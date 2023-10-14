#include "Skybox.h"

#include <glad/glad.h>

#include "../Texture.h"
#include "../Material.h"
#include "../Renderer.h"
#include "../components/meshes/Cube.h"

Skybox::Skybox()
{
    const std::shared_ptr<Texture> texture = std::make_shared<Texture>(TextureType::cubemap, "./Data/images/skybox/cubemap_clouds_");
    const std::shared_ptr<Material> matSky = std::make_shared<Material>("./Engine/shaders/vertexSkybox.glsl", "./Engine/shaders/fragmentSkybox.glsl", MaterialType::Cubemap);

    matSky->SetTexture(texture);

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
