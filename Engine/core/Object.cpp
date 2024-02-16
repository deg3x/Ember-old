#include "engine_pch.h"
#include "Object.h"

#include "core/Model.h"
#include "core/components/Component.h"
#include "core/components/Transform.h"
#include "core/components/Mesh.h"

Object::Object(const char* objName)
{
    isActive  = true;
    transform = CreateComponent<Transform>();
    name      = objName;
    uid       = nextAvailableID;

    nextAvailableID++;
}

Object::~Object()
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->owner = nullptr;
    }
}

void Object::Tick()
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->Tick();
    }
}

void Object::LoadModel(const char* path)
{
    // May need to change this in the future in order to include whole scenes
    const std::vector<std::shared_ptr<Mesh>> meshes = Model::Load(path);
    for (const std::shared_ptr<Mesh>& mesh : meshes)
    {
        AddComponent(mesh);
    }
}
