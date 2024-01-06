#include "Object.h"

#include "core/Model.h"
#include "core/components/Component.h"
#include "core/components/Transform.h"
#include "core/components/meshes/Mesh.h"

Object::Object()
{
    transform = CreateComponent<Transform>();
}

Object::~Object()
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->parent = nullptr;
    }
}

void Object::Tick() const
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->Tick();
    }
}

void Object::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    const std::vector<std::shared_ptr<Mesh>> meshes = GetComponents<Mesh>();

    for (const std::shared_ptr<Mesh>& mesh : meshes)
    {
        if (mesh == nullptr)
        {
            continue;
        }

        mesh->Draw(camera, lights);
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
