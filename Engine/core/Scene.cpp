#include "Scene.h"

#include "objects/Object.h"
#include "components/Camera.h"
#include "components/lights/Light.h"

Scene* Scene::Active = nullptr;

Scene::~Scene()
{
    if (Active == this)
    {
        Active = nullptr;
    }
}

void Scene::Use()
{
    Active = this;
}

void Scene::Tick() const
{
    // Opaque object tick and rendering
    for (const std::shared_ptr<Object>& object : obj_queue_opaque)
    {
        object->Tick();
        object->Draw(camera, lights);
    }
    
    // Transparent object tick and rendering
    for (const std::shared_ptr<Object>& object : obj_queue_transparent)
    {
        object->Tick();
        object->Draw(camera, lights);
    }
}

void Scene::AddObject(const std::shared_ptr<Object>& object, ObjectType type)
{
    if (obj_queue_opaque.contains(object) || obj_queue_transparent.contains(object))
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
        obj_queue_opaque.insert(object);
        break;
    case ObjectType::TRANSPARENT:
        obj_queue_transparent.insert(object);
        break;
    }
}

void Scene::RemoveObject(const std::shared_ptr<Object>& object)
{
    if (obj_queue_opaque.contains(object))
    {
        obj_queue_opaque.erase(object);
        
        return;
    }

    if (obj_queue_transparent.contains(object))
    {
        obj_queue_transparent.erase(object);
        
        return;
    }
}
