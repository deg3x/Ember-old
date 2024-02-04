#include "engine_pch.h"
#include "World.h"

#include "glad/glad.h"

#include "components/Mesh.h"
#include "core/Object.h"
#include "core/Renderer.h"
#include "core/components/Camera.h"
#include "core/components/Light.h"
#include "core/components/Transform.h"
#include "utils/Types.h"
#include "utils/ObjectPrimitive.h"

std::unordered_set<std::shared_ptr<Object>> World::objQueueOpaque;
std::unordered_set<std::shared_ptr<Object>> World::objQueueTransparent;
std::unordered_set<std::shared_ptr<Light>> World::lights;
std::shared_ptr<Camera> World::camera;

void World::Initialize()
{
    const std::shared_ptr<Object> cameraObject = std::make_shared<Object>("Camera");
    cameraObject->CreateComponent<Camera>();
    cameraObject->transform->position = glm::vec3(0.0f, 3.0f, 3.0f);
    cameraObject->transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);

    /*const std::shared_ptr<Object> bunnyObject = std::make_shared<Object>("Stanford Bunny");
    bunnyObject->transform->position = glm::vec3(0.0f, 0.2f, 0.0f);
    bunnyObject->transform->scale = glm::vec3(0.5f, 0.5f, 0.5f);
    bunnyObject->LoadModel("./Data/models/bunny.obj");*/

    const std::shared_ptr<Object> dirLightObject = std::make_shared<Object>("Directional Light");
    dirLightObject->CreateComponent<Light>();
    dirLightObject->transform->rotation.x = 30.0f;
    dirLightObject->transform->rotation.y = -30.0f;

    AddObject(cameraObject);
    //AddObject(bunnyObject);
    AddObject(dirLightObject);

    ObjectPrimitive::InstantiateSphere();
    ObjectPrimitive::InstantiateSkybox();
    ObjectPrimitive::InstantiateEditorGrid();

    // Startup renderer state
    constexpr Color clear_color = {0.16f, 0.15f, 0.18f, 1.00f};
    Renderer::SetDepthTestEnabled(true);
    Renderer::SetDepthTestFunc(GL_LESS);
    Renderer::SetBlendingEnabled(true);
    Renderer::SetBlendingFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    Renderer::SetBlendingOp(GL_FUNC_ADD);
    Renderer::SetClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

void World::Tick()
{
    // Opaque object tick and rendering
    for (const std::shared_ptr<Object>& object : objQueueOpaque)
    {
        if (!object->isActive)
        {
            continue;
        }
        
        object->Tick();
        object->Draw(camera, lights);
    }
    
    // Transparent object tick and rendering
    for (const std::shared_ptr<Object>& object : objQueueTransparent)
    {
        if (!object->isActive)
        {
            continue;
        }
        
        object->Tick();
        object->Draw(camera, lights);
    }
}

void World::AddObject(const std::shared_ptr<Object>& object)
{
    if (objQueueOpaque.contains(object) || objQueueTransparent.contains(object))
    {
        return;
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
    
    const std::shared_ptr<Camera> cameraComponent = object->GetComponent<Camera>();
    if (cameraComponent != nullptr)
    {
        camera = cameraComponent;
    }

    const std::shared_ptr<Light> lightComponent = object->GetComponent<Light>();
    if (lightComponent != nullptr)
    {
        AddLight(lightComponent);
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

void World::AddLight(const std::shared_ptr<Light>& light)
{
    // Extremely ugly solution
    // Fix in the future before the universe explodes
    bool found = false;
    for (const std::shared_ptr<Object>& object : objQueueOpaque)
    {
        if (object.get() == light->GetOwner())
        {
            found = true;
        }
    }
    for (const std::shared_ptr<Object>& object : objQueueTransparent)
    {
        if (object.get() == light->GetOwner())
        {
            found = true;
        }
    }
    if (!found)
    {
        return;
    }

    lights.insert(light);
}

void World::RemoveLight(const std::shared_ptr<Light>& light)
{
    lights.extract(light);
}
