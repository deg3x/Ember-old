#include "engine_pch.h"
#include "glad/glad.h"
#include "World.h"

#include "components/Mesh.h"
#include "core/Renderer.h"
#include "core/Object.h"
#include "core/components/Camera.h"
#include "core/components/Light.h"
#include "core/components/Transform.h"
#include "core/materials/MaterialBlinnPhong.h"
#include "utils/primitives/EditorGrid.h"
#include "utils/primitives/Skybox.h"

std::unordered_set<std::shared_ptr<Object>> World::objQueueOpaque;
std::unordered_set<std::shared_ptr<Object>> World::objQueueTransparent;
std::vector<std::shared_ptr<Light>> World::lights;
std::shared_ptr<Camera> World::camera;

void World::Initialize()
{
    const std::shared_ptr<Object> cameraObject = std::make_shared<Object>("Camera");
    cameraObject->CreateComponent<Camera>();
    cameraObject->transform->position = glm::vec3(0.0f, 3.0f, 3.0f);
    cameraObject->transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);

    const std::shared_ptr<MaterialBlinnPhong> defaultMat = std::make_shared<MaterialBlinnPhong>();

    const std::shared_ptr<Object> bunnyObject = std::make_shared<Object>("Stanford Bunny");
    bunnyObject->transform->position = glm::vec3(0.0f, 0.2f, 0.0f);
    bunnyObject->transform->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    bunnyObject->LoadModel("./Data/models/bunny.obj");

    const std::shared_ptr<Object> dirLightObject = std::make_shared<Object>("Directional Light");
    dirLightObject->CreateComponent<Light>();
    dirLightObject->transform->rotation.x = 30.0f;
    dirLightObject->transform->rotation.y = -30.0f;

    dirLightObject->GetComponent<Light>()->SetShaderProperties(*defaultMat->GetShader());

    AddObject(cameraObject);
    AddObject(bunnyObject);
    AddObject(dirLightObject);

    Skybox::Instantiate();
    EditorGrid::Instantiate();
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

void World::AddObject(const std::shared_ptr<Object>& object)
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

    const std::shared_ptr<Mesh> meshComponent = object->GetComponent<Mesh>();
    if (meshComponent != nullptr && meshComponent->meshType == MeshType::TRANSPARENT)
    {
        objQueueTransparent.insert(object);
    }
    else
    {
        objQueueOpaque.insert(object);
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
