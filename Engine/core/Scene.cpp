#include "Scene.h"

#include "Material.h"
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
    for (const std::shared_ptr<Object>& object : objects)
    {
        object->Tick();
        object->Draw(camera, lights);
    }
}

void Scene::AddObject(const std::shared_ptr<Object>& object)
{
    if (objects.contains(object))
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

    objects.insert(object);
}

void Scene::RemoveObject(const std::shared_ptr<Object>& object)
{
    if (!objects.contains(object))
    {
        return;
    }

    objects.erase(object);
}
