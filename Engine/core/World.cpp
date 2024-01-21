#include "glad/glad.h"
#include "World.h"

#include "core/Renderer.h"
#include "core/objects/Object.h"
#include "core/objects/EditorGrid.h"
#include "core/objects/Skybox.h"
#include "core/components/Camera.h"
#include "core/components/lights/Light.h"
#include "core/components/Transform.h"
#include "core/components/meshes/Plane.h"
#include "core/components/lights/DirectionalLight.h"
#include "core/materials/MaterialBlinnPhong.h"

std::unordered_set<std::shared_ptr<Object>> World::objQueueOpaque;
std::unordered_set<std::shared_ptr<Object>> World::objQueueTransparent;
std::vector<std::shared_ptr<Light>> World::lights;
std::shared_ptr<Camera> World::camera;

void World::Initialize()
{
    const std::shared_ptr<Object> cameraObject = std::make_shared<Object>();
    cameraObject->CreateComponent<Camera>();
    cameraObject->transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraObject->transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);

    const std::shared_ptr<MaterialBlinnPhong> defaultMat = std::make_shared<MaterialBlinnPhong>();

    const std::shared_ptr<Object> bunnyObject = std::make_shared<Object>();
    bunnyObject->transform->position = glm::vec3(0.0f, -0.33f, 0.0f);
    bunnyObject->transform->scale =glm::vec3(0.5f, 0.5f, 0.5f);
    bunnyObject->LoadModel("./Data/models/bunny.obj");

    const std::shared_ptr<EditorGrid> grid = std::make_shared<EditorGrid>();
    grid->transform->position = glm::vec3(0.0f, -0.51f, 0.0f);
    grid->transform->scale = glm::vec3(100.0f, 1.0f, 100.0f);

    const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>();

    const std::shared_ptr<Object> dirLightObject = std::make_shared<Object>();
    dirLightObject->CreateComponent<DirectionalLight>();
    dirLightObject->transform->rotation.x = 30.0f;
    dirLightObject->transform->rotation.y = -30.0f;

    dirLightObject->GetComponent<Light>()->SetShaderProperties(*defaultMat->GetShader());

    AddObject(cameraObject, ObjectType::OPAQUE);
    AddObject(bunnyObject, ObjectType::OPAQUE);
    AddObject(dirLightObject, ObjectType::OPAQUE);
    AddObject(skybox, ObjectType::OPAQUE);
    AddObject(grid, ObjectType::TRANSPARENT);
}

void World::Tick()
{
    Renderer::SetDepthTestMask(true);
    Renderer::SetFaceCullingEnabled(false);
    
    // Opaque object tick and rendering
    for (const std::shared_ptr<Object>& object : objQueueOpaque)
    {
        object->Tick();
        object->Draw(camera, lights);
    }

    Renderer::SetDepthTestMask(false);
    Renderer::SetFaceCullingEnabled(true);
    Renderer::SetFaceCullingMode(GL_BACK);
    
    // Transparent object tick and rendering
    for (const std::shared_ptr<Object>& object : objQueueTransparent)
    {
        object->Tick();
        object->Draw(camera, lights);
    }

    Renderer::SetDepthTestMask(true);
}

void World::AddObject(const std::shared_ptr<Object>& object, ObjectType type)
{
    if (objQueueOpaque.contains(object) || objQueueTransparent.contains(object))
    {
        return;
    }
    
    const std::shared_ptr<Camera> cameraComponent = object->GetComponent<Camera>();
    if (cameraComponent != nullptr)
    {
        camera = cameraComponent;
    }

    const std::shared_ptr<Light> lightComponent = object->GetComponent<Light>();
    if (lightComponent != nullptr)
    {
        lights.push_back(lightComponent);
    }

    switch (type)
    {
    case ObjectType::OPAQUE:
        objQueueOpaque.insert(object);
        break;
    case ObjectType::TRANSPARENT:
        objQueueTransparent.insert(object);
        break;
    }
}

void World::RemoveObject(const std::shared_ptr<Object>& object)
{
    if (objQueueOpaque.contains(object))
    {
        objQueueOpaque.erase(object);
        
        return;
    }

    if (objQueueTransparent.contains(object))
    {
        objQueueTransparent.erase(object);
        
        return;
    }
}
